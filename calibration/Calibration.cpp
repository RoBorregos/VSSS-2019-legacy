#include "Calibration.h"

// Static variables declaration
std::string Calibration::screenName;
cv::Mat Calibration::hsv_image, Calibration::mask, Calibration::result;
cv::Mat *Calibration::original;
int Calibration::hueMin, Calibration::hueMax, Calibration::satMin, Calibration::satMax,
    Calibration::valMin, Calibration::valMax;

Calibration::Calibration(std::string screenName, cv::Mat &image){
  // Assigning constructor values
  this -> screenName = screenName;
  original = &image;

  // Sets maximum slider values to 255
  hueMax = 255; satMax = 255; valMax = 255;
  
  // Creates a window display with 'screenName' as name
  cv::namedWindow(screenName, cv::WINDOW_AUTOSIZE);
  // Stores a hsv copy of the original image as 'hsv_image'
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

  // Creates 6 sliders in our window display with a maxium value of 255 and a different
  // callBack function each of them
  cv::createTrackbar("Low  H", screenName, &hueMin, 255, this->onHueMinTrackBar);
  cv::createTrackbar("High H", screenName, &hueMax, 255, this->onHueMaxTrackBar);
  cv::createTrackbar("Low  S", screenName, &satMin, 255, this->onSatMinTrackBar);
  cv::createTrackbar("High S", screenName, &satMax, 255, this->onSatMaxTrackBar);
  cv::createTrackbar("Low  V", screenName, &valMin, 255, this->onValMinTrackBar);
  cv::createTrackbar("High V", screenName, &valMax, 255, this->onValMaxTrackBar);

  // Displays the original image with the sliders, once
  update();
  // cv::imshow(screenName, *original);
}

void Calibration::onHueMinTrackBar(int, void*){
  // Minimum Hue cannot be higher than maximum Hue
  hueMin = std::min(hueMax-1, hueMin);
  cv::setTrackbarPos("Low  H", screenName, hueMin);
  // Updates and displays the resulting image
  update();
}

void Calibration::onHueMaxTrackBar(int, void*){
  // Maximum Hue cannot be lower than minimum Hue
  hueMax = std::max(hueMax, hueMin+1);
  cv::setTrackbarPos("High H", screenName, hueMax);
  // Updates and displays the resulting image
  update();
}

void Calibration::onSatMinTrackBar(int, void*){
  // Minimum Saturation cannot be higher than maximum Saturation
  satMin = std::min(satMax-1, satMin);
  cv::setTrackbarPos("Low  S", screenName, satMin);
  // Updates and displays the resulting image
  update();
}

void Calibration::onSatMaxTrackBar(int, void*){
  // Maximum Saturation cannot be lower than minimum Sturation
  satMax = std::max(satMax, satMin+1);
  cv::setTrackbarPos("High S", screenName, satMax);
  // Updates and displays the resulting image
  update();
}

void Calibration::onValMinTrackBar(int, void*){
  // Minimum Value cannot be higher than maximum Value
  valMin = std::min(valMax-1, valMin);
  cv::setTrackbarPos("Low  V", screenName, valMin);
  // Updates and displays the resulting image
  update();
}

void Calibration::onValMaxTrackBar(int, void*){
  // Maximum Value cannot be lower than minimum Value
  valMax = std::max(valMax, valMin+1);
  cv::setTrackbarPos("High V", screenName, valMax);
  // Updates and displays the resulting image
  update();
}

void Calibration::update(){
  // Sets result image as an empty image with original size and type
  result = cv::Mat::zeros((*original).size(), (*original).type());
  // Updates the hsv_image
  cv::cvtColor(*original, hsv_image, cv::COLOR_BGR2HSV);
  // Updates mask values with the corresponding H,S,V
  cv::inRange(hsv_image, cv::Scalar(hueMin, satMin, valMin), cv::Scalar(hueMax, satMax, valMax), mask);
  // Dilates the mask
  cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1));
  // Combines the mask with the original image, as result we get only the filtered colors 
  (*original).copyTo(result, mask);
  // Displays the resulting image
  cv::imshow(screenName, result);
}