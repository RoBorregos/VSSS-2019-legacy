#include "Vision.h"

Vision::Vision(cv::Mat &image, std::string teamColor, std::vector<Shape> &allies, std::vector<Shape> &enemies, Shape &ball){
  original = &image;

  width = (*original).cols;
  height = (*original).rows;

  std::cout << width << " " << height << std::endl;

  setLimits();
  setHSV(orange, "Orange");
  setHSV(blue, "Blue");
  setHSV(yellow, "Yellow");
  setHSV(red, "Red");
  setHSV(green, "Green");
  setHSV(pink, "Pink");

  this -> teamColor = teamColor;
  this -> enemies = enemies;
  this -> allies = allies;
  this -> ball = ball;
  
  drawSet = false;
}

void Vision::setLimits(){
  // Reads the file
  std::ifstream file("./vision/calibration/limits.txt");
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  std::string aux;
  float value;

  while(file >> aux >> value){
    if(aux == "minArea")
      minArea = value;
    else if(aux == "maxArea")
      maxArea = value;
    else if(aux == "maxDistance")
      maxDistance = value;
    else
      std::cout << "Limit was not found in the file" << std::endl;
  }

}

void Vision::setHSV(hsv &hsvColor, std::string color){
  // Reads the file
  std::ifstream file("./vision/calibration/colors.txt");
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  int hMin, hMax, sMin, sMax, vMin, vMax;
  std::string c;
  
  // Reads each line in colors.txt and stops when c == color
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

  std::cout << hMin << hMax << sMin << sMax << vMin << vMax << std::endl;
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

  // std::cout << "contours size = " << contours.size() << std::endl;
  return contours;
}

void Vision::fixContours(std::vector<std::vector<cv::Point> > &contours){
  for(size_t i = 0; i< contours.size(); i++){
    float area = cv::contourArea(contours[i]); // image area's percentage
    if(area < minArea || area > maxArea)
      contours.erase(contours.begin() + int(i--));
  }
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
  
  // drawContours(contours, mc);
  return mc;
}

c_pair Vision::getCentroidPair(std::vector<cv::Point2f> c_color, std::vector<cv::Point2f> c_target){
  double minDistance = INT_MAX;

  if(c_color.size() < 1 || c_target.size() < 1)
    return c_pair();

  c_pair cp(c_color[0], c_target[0]); 
  
  for(int i = 0; i < c_color.size(); i++){
    for(int j = 0; j < c_target.size(); j++){
      double distance = sqrt(pow(c_color[i].y - c_target[j].y, 2) + pow(c_color[i].x - c_target[j].x, 2));
      if(distance < minDistance && distance < maxDistance){
        minDistance = distance;
        cp.c_color = c_color[i];
        cp.c_teamColor = c_target[j];
      }
    }
  }

  if(minDistance == INT_MAX){
    std::cout << "WARNING: Rob could not be found. This could happened because the distance was to big\n";
    return c_pair();
  }
  
  return cp;
}

void Vision::updateValues(Shape &f, c_pair cp){
  double lastX, lastY, xlen, ylen, angle;
  long long timeDiff; 

  if(!centroidIsEmpty(cp)){
    lastX = f.currentPos.x;
    lastY = f.currentPos.y;
    xlen = cp.c_color.x - cp.c_teamColor.x;
    ylen = cp.c_color.y - cp.c_teamColor.y;
    xlen = xlen == 0 ? 0.000001 : xlen;
    ylen = ylen == 0 ? 0.000001 : ylen;
    
    f.currentPos.x = xlen/2 + cp.c_teamColor.x;
    f.currentPos.y = ylen/2 + cp.c_teamColor.y;
    
    if(&f != &ball){
      angle = (atan(fabs(ylen) / fabs(xlen)) * 180 / PI);
      if(cp.c_color.x < cp.c_teamColor.x)
        f.ori = 180 + (cp.c_color.y > cp.c_teamColor.y ? angle : -angle);
      else  
        f.ori = cp.c_color.y > cp.c_teamColor.y ? 360 - angle : angle;
    }
    f.ori = fabs(360-f.ori);

    timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - f.lastTime).count();
    f.dx = (f.currentPos.x - lastX) / timeDiff;
    f.dy = (f.currentPos.y - lastY) / timeDiff;

    // --------- TESTING DX & DY --------------------
    std::chrono::time_point<std::chrono::system_clock> timeNow = std::chrono::system_clock::now();
    f.lastTime = timeNow;
  }

  std::cout << "x: " << f.currentPos.x << "  y: " << f.currentPos.y << "  dx: " << f.dx << "  dy: " << f.dy;
  std::cout << "  ori: " << f.ori << std::endl;

  draw(f.currentPos, cp, f.ori);  
}

bool Vision::centroidIsEmpty(c_pair cp){
  return cp.c_color.x == 0 && cp.c_color.y == 0 && cp.c_teamColor.x == 0 && cp.c_teamColor.y == 0;
}

void Vision::draw(Point ref, c_pair cp, float ori){
  const float xExtension = 50;

  cv::Point2f center = cv::Point2f(ref.x, ref.y), aux;
  cv::circle(drawnImg, center, 3, cv::Scalar(0, 255, 0), -1);
  cv::circle(drawnImg, cp.c_teamColor, 3, cv::Scalar(255, 255, 255), -1);
  cv::circle(drawnImg, cp.c_color, 3, cv::Scalar(255, 255, 255), -1);

  aux.x = center.x + (xExtension * cos(ori * PI / 180));
  aux.y = center.y + (xExtension * sin(ori * PI / 180));

  if(drawSet) 
    cv::line(drawnImg, center, aux, cv::Scalar(0, 0, 255), 1);
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
  // podemos no utilizar c_blue & c_yellow

  if(drawSet) drawnImg = (*original).clone();
  std::cout << "Rob1\n";
  updateValues(allies[0], getCentroidPair(c_red, c_team));
  std::cout << "Rob2\n";
  updateValues(allies[1], getCentroidPair(c_green, c_team));
  std::cout << "Rob3\n";
  updateValues(allies[2], getCentroidPair(c_pink, c_team));
  std::cout << "Ball\n";
  updateValues(ball, getCentroidPair(c_orange, c_orange));
  std::cout << "-------------------------------\n";
  
  if(drawSet) cv::imshow("image", drawnImg);
}

void Vision::setDraw(bool b){
  drawSet = b;
}