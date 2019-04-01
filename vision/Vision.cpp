#include "Vision.h"

// Static attributes
cv::Mat *Vision::original, Vision::masked;
hsv Vision::orange, Vision::blue, Vision::yellow, Vision::red, Vision::green, Vision::pink;
foo Vision::rob1, Vision::rob2, Vision::rob3, Vision::enm1, Vision::enm2, Vision::enm3, Vision::ball;
double Vision::width, Vision::height, *Vision::circleMinArea, *Vision::circleMaxArea, *Vision::maxHyp;
std::vector<cv::Vec4i> Vision::hierarchy;
std::string Vision::teamColor;


Vision::Vision(cv::Mat &image, std::string color){
  original = &image;

  width = (*original).cols;
  height = (*original).rows;

  setHSV(orange, "Orange");
  setHSV(blue, "Blue");
  setHSV(yellow, "Yellow");
  setHSV(red, "Red");
  setHSV(green, "Green");
  setHSV(pink, "Pink");

  if(color != "blue" && color != "yellow"){
    std::cout << "WARNING: provided teamColor doesn't exist, please verify it!!\n(blue color would be selected by default)";
  }

  teamColor = color == "yellow" ? color : "blue";
}

void Vision::settings(double &cMinArea, double &cMaxArea, double &maxH){
  circleMinArea = &cMinArea;
  circleMaxArea = &cMaxArea;
  maxHyp = &maxH;
}

void Vision::setHSV(hsv &hsvColor, std::string color){
  // Reads the file
  std::ifstream file("calibration/Values.txt");
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  int hMin, hMax, sMin, sMax, vMin, vMax;
  std::string c;
  
  // Reads each line in values.txt and stops when c == color
  while(file >> c >> hMin >> hMax >> sMin >> sMax >> vMin >> vMax && c != color);

  // Consoles out if the target color was not found
  if (c != color)
    std::cout << color << " color was not found in the file\n";

  // Sets the hsv values found in the file
  hsvColor.hMin = hMin;
  hsvColor.hMax = hMax;
  hsvColor.sMin = sMin;
  hsvColor.sMax = sMax;
  hsvColor.vMin = vMin;
  hsvColor.vMax = vMax;
}

void Vision::updateMask(hsv c){
  cv::Mat hsv_image;

  // Updates the hsv_image
  cv::cvtColor(*original, hsv_image, cv::COLOR_BGR2HSV);

  // Updates mask values with the corresponding H,S,V
  cv::inRange(hsv_image, cv::Scalar(c.hMin, c.sMin, c.vMin), cv::Scalar(c.hMax, c.sMax, c.vMax), masked);
}

std::vector<std::vector<cv::Point> > Vision::getContours(hsv color){
  std::vector<std::vector<cv::Point> > contours;
  
  updateMask(color);

  cv::findContours(masked, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
  fixContours(contours);

  std::cout << "contours size = " << contours.size() << std::endl;
  return contours;
}

void Vision::fixContours(std::vector<std::vector<cv::Point> > &contours){
  for(size_t i = 0; i< contours.size(); i++){
    double area = cv::contourArea(contours[i])/(width * height) * 100; // image area's percentage
    if(area < *circleMinArea || area > *circleMaxArea)
      contours.erase(contours.begin() + int(i--));
  }
}

std::vector<cv::Point2f> Vision::getCentroids(hsv color){
  std::vector<std::vector<cv::Point> > contours = getContours(color);

  if(contours.size() < 1)
    std::cout << "WARNING: hsv color contours is null or empty. This will unchain several problems.\n";

  // get the moments
  std::vector<cv::Moments> mu(contours.size());
  for(size_t i = 0; i < contours.size(); i++)
    mu[i] = moments(contours[i], false);

  // get the centroid of figures.
  std::vector<cv::Point2f> mc(contours.size());
  for(size_t i = 0; i<contours.size(); i++)
    mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00 , mu[i].m01 / mu[i].m00);
  
  drawContours(contours, mc);
  
  return mc;
}

void Vision::drawContours(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Point2f> centroids){
  cv::Mat drawing = cv::Mat::zeros((*original).size(), CV_8UC3);
  
  for(size_t i = 0; i< contours.size(); i++){
    cv::drawContours(drawing, contours, int(i), cv::Scalar(255,255,255), 1, 8, hierarchy, 0, cv::Point());
    circle( drawing, centroids[i], 2, cv::Scalar(255,255,255), -1, 8, 0);
  }

  namedWindow("Contours", cv::WINDOW_AUTOSIZE);
  imshow("Contours", drawing);
  cv::waitKey(0);
}

void Vision::update(){
  std::vector<cv::Point2f> c_orange, c_blue, c_yellow, c_red, c_green, c_pink, c_team;
  c_orange = getCentroids(orange);
  c_blue = getCentroids(blue);
  c_yellow = getCentroids(yellow);
  c_red = getCentroids(red);
  c_green = getCentroids(green);
  c_pink = getCentroids(pink);
  c_team = teamColor == "blue" ? c_blue : c_yellow;

  updateValues(rob1, getCentroidPair(c_red, c_team));
  updateValues(rob2, getCentroidPair(c_green, c_team));
  updateValues(rob3, getCentroidPair(c_pink, c_team));
  updateValues(ball, getCentroidPair(c_orange, c_orange));
}

c_pair Vision::getCentroidPair(std::vector<cv::Point2f> c_color, std::vector<cv::Point2f> c_target){
  double minHyp = INT_MAX;
  c_pair cp(c_color[0], c_target[0]); 
  
  for(int i = 0; i < c_color.size(); i++){
    for(int j = 0; j < c_target.size(); j++){
      double hyp = sqrt(pow(c_color[i].y - c_target[j].y, 2) + pow(c_color[i].x - c_target[j].x, 2));
      if(hyp < minHyp && hyp < *maxHyp){
        minHyp = hyp;
        cp.c_color = c_color[i];
        cp.c_teamColor = c_target[j];
      }
    }
  }

  if(minHyp == INT_MAX)
    std::cout << "WARNING: Rob could not be found. This could happened because the hyp was to big\n";

  return cp;
}

void Vision::updateValues(foo &f, c_pair cp){
  double lastX, lastY, xlen, ylen, timeDiff;

  lastX = f.x;
  lastY = f.y;
  xlen = cp.c_color.x - cp.c_teamColor.x;
  ylen = cp.c_color.y - cp.c_teamColor.y;
  xlen = xlen == 0 ? 0.0001 : xlen;
  ylen = ylen == 0 ? 0.0001 : ylen;
  
  f.x = xlen/2 + cp.c_teamColor.x;
  f.y = ylen/2 + cp.c_teamColor.y;
  
  if(&f != &ball)
    f.ori = (atan(fabs(ylen) / fabs(xlen)) * 180 / PI) - 45;

  if(f.firstTimeFlag){
    f.dx = 0;
    f.dy = 0;
    f.firstTimeFlag = false;
  }else{
    timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - *f.lastTime).count();
    f.dx = (f.x - lastX) / timeDiff;
    f.dy = (f.y - lastY) / timeDiff;
  }

  std::cout << "x: " << f.x << "  y: " << f.y << "  dx: " << f.dx << "  dy: " << f.dy;
  std::cout << "  ori: " << f.ori << std::endl;

  // --------- TESTING DX & DY --------------------
  // std::chrono::time_point<std::chrono::system_clock> timeNow = std::chrono::system_clock::now();
  // f.lastTime = &timeNow;

  // // ------------------------------------
  // std::this_thread::sleep_for(std::chrono::milliseconds(500));
  // lastX = f.x;
  // lastY = f.y;
  // f.x += 120;
  // f.y += 80;
  // timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - *f.lastTime).count();
  // f.dx = (f.x - lastX) / timeDiff;
  // f.dy = (f.y - lastY) / timeDiff;

  // std::cout << "------------------------------------------\n";
  // std::cout << "x: " << f.x << "  y: " << f.y << "  dx: " << f.dx << "  dy: " << f.dy;
  // std::cout << "  ori: " << f.ori << std::endl;

}
