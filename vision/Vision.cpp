#include "Vision.h"

// Static attributes
cv::Mat *Vision::original, Vision::masked;
hsv Vision::orange, Vision::blue, Vision::yellow, Vision::red, Vision::green, Vision::pink, *Vision::teamColor;
foo Vision::rob1, Vision::rob2, Vision::rob3, Vision::enm1, Vision::enm2, Vision::enm3, Vision::ball;


Vision::Vision(cv::Mat &image, std::string color){
  original = &image;

  setHSV(orange, "Orange");
  setHSV(blue, "Blue");
  setHSV(yellow, "Yellow");
  setHSV(red, "Red");
  setHSV(green, "Green");
  setHSV(pink, "Pink");

  if(color != "blue" && color != "yellow"){
    std::cout << "WARNING: provided teamColor doesn't exist, please verify it!!\n(blue color would be selected by default)";
  }

  teamColor = color == "yellow" ? &yellow : &blue;
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

std::vector<std::vector<cv::Point> > Vision::getContours(){
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  updateMask(blue);
  cv::findContours(masked, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));

  cv::Mat drawing = cv::Mat::zeros( (*original).size(), CV_8UC3 );
  for(size_t i = 0; i< contours.size(); i++){
    drawContours( drawing, contours, int(i), cv::Scalar(255,255,255), 1, 8, hierarchy, 0, cv::Point());
    namedWindow( "Contours", cv::WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    cv::waitKey(0);
  }
  return contours;
}
