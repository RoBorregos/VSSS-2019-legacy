#include "Calibration.h"

#include <cmath>
#include <utility>
#include <vector>

#define ENTER 13
#define COLORS_FILE_NAME "colors.txt"
#define FILTER_SAMPLE 2
#define LIMITS_FILE_NAME "limits.txt"
#define MIN_MAX_HSV_RANGE 100
#define MIN_MAX_AREA_RANGE 100

// Static variables declaration
cv::Scalar Calibration::scalarColor;
std::ifstream infile;
std::string Calibration::screenName, Calibration::logText, Calibration::currentColor;
cv::Mat Calibration::hsv_image, Calibration::mask, Calibration::result;
cv::Mat *Calibration::original;
int Calibration::hueMin, Calibration::hueMax, Calibration::satMin, Calibration::satMax,
    Calibration::valMin, Calibration::valMax;

std::vector<cv::Vec3b> pixels;
bool getSquareLimits = false;
bool getDistanceLimits = false;
bool squareCorner = false;
bool getInitialDistancePixel = true;
std::pair<int, int> squareUpperLeftCorner(-1, -1);
std::pair<int, int> squareBottomRightCorner(-1, -1);
std::pair<int, int> initialDistancePixel(-1, -1);
std::pair<int, int> finalDistancePixel(-1, -1);

void mouseEvent(int event, int x, int y, int flags, void* param) {
  cv::Mat* image = (cv::Mat*)param;

  switch (event) {
      case CV_EVENT_LBUTTONDOWN:
        if (getSquareLimits) {
          if (!squareCorner) {
            squareUpperLeftCorner = std::make_pair(x, y);
            squareBottomRightCorner = std::make_pair(-1, -1);
          } else {
            squareBottomRightCorner = std::make_pair(x, y);
          }
          squareCorner = !squareCorner;
        } else if (getDistanceLimits) {
          if (getInitialDistancePixel) {
            initialDistancePixel = std::make_pair(x, y);
            finalDistancePixel = std::make_pair(-1, -1);
          } else {
            finalDistancePixel = std::make_pair(x, y);
          }
          getInitialDistancePixel = !getInitialDistancePixel;
        } else {
          pixels.push_back(image->at<cv::Vec3b>(y,x));
        }
        break;
  }
}

Calibration::Calibration(std::string screenName, cv::Mat &image){
  // Assigning constructor values
  this -> screenName = screenName;
  original = &image;

  currentColor = WHITE;
  scalarColor = cv::Scalar(255, 255, 0255);
  logText = "";

  // Sets maximum slider values to 255
  hueMax = 255; satMax = 255; valMax = 255;
  
  // Creates a window display with 'screenName' as name
  cv::namedWindow(screenName, cv::WINDOW_AUTOSIZE);
  // Stores a hsv copy of the original image as 'hsv_image'
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);
  cv::setMouseCallback(screenName, mouseEvent, &hsv_image);

  // Creates 6 sliders in our window display with a maxium value of 255
  cv::createTrackbar("Low  H", screenName, &hueMin, 255, this->onHueMinTrackBar);
  cv::createTrackbar("High H", screenName, &hueMax, 255, this->onHueMaxTrackBar);
  cv::createTrackbar("Low  S", screenName, &satMin, 255, this->onSatMinTrackBar);
  cv::createTrackbar("High S", screenName, &satMax, 255, this->onSatMaxTrackBar);
  cv::createTrackbar("Low  V", screenName, &valMin, 255, this->onValMinTrackBar); 
  cv::createTrackbar("High V", screenName, &valMax, 255, this->onValMaxTrackBar);

  // Displays the original image with the sliders, once
  update();
}

void Calibration::updateTrackbars() {
  cv::setTrackbarPos("Low  H", screenName, hueMin);
  cv::setTrackbarPos("High H", screenName, hueMax);
  cv::setTrackbarPos("Low  S", screenName, satMin);
  cv::setTrackbarPos("High S", screenName, satMax);
  cv::setTrackbarPos("Low  V", screenName, valMin);
  cv::setTrackbarPos("High V", screenName, valMax);
}

void Calibration::onHueMinTrackBar(int, void*){
  // Minimum Hue cannot be higher than maximum Hue
  hueMin = std::min(hueMax-1, hueMin);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

void Calibration::onHueMaxTrackBar(int, void*){
  // Maximum Hue cannot be lower than minimum Hue
  hueMax = std::max(hueMax, hueMin+1);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

void Calibration::onSatMinTrackBar(int, void*){
  // Minimum Saturation cannot be higher than maximum Saturation
  satMin = std::min(satMax-1, satMin);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

void Calibration::onSatMaxTrackBar(int, void*){
  // Maximum Saturation cannot be lower than minimum Sturation
  satMax = std::max(satMax, satMin+1);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

void Calibration::onValMinTrackBar(int, void*){
  // Minimum Value cannot be higher than maximum Value
  valMin = std::min(valMax-1, valMin);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

void Calibration::onValMaxTrackBar(int, void*){
  // Maximum Value cannot be lower than minimum Value
  valMax = std::max(valMax, valMin+1);
  updateTrackbars();
  // Updates and displays the resulting image
  update();
}

bool Calibration::listenKey(){
  char key = (char) cv::waitKey(30);

  switch(key){
    case 'o':
      currentColor = ORANGE;
      readColor(currentColor);
      break;
    case 'b':
      currentColor = BLUE;
      readColor(currentColor);
      break;
    case 'y':
      currentColor = YELLOW;
      readColor(currentColor);
      break;
    case 'r':
      currentColor = RED;
      readColor(currentColor);
      break;
    case 'g':
      currentColor = GREEN;
      readColor(currentColor);
      break;
    case 'p':
      currentColor = PINK;
      readColor(currentColor);
      break;
    case 's':
      getSquareLimits = !getSquareLimits;
      if (getDistanceLimits) {
        getDistanceLimits = false;
      }
      break;
    case 'd':
      getDistanceLimits = !getDistanceLimits;
      if (getSquareLimits) {
        getSquareLimits = false;
      }
      break;
    case 'x':
      pixels.clear();
      break;
    case 'l':
      updateTrackbars();
      break;
    case ENTER:
      if (getSquareLimits || getDistanceLimits) {
        saveLimits();
      } else if (currentColor != WHITE) {
        if (saveColor()) {
          logText = "Color saved!";
        } else {
          logText = "Error :(";
        }
      }
      break;
    default:
      if(key != -1){
        readColor("");
        currentColor = WHITE;
        logText = "";
      }
  }
  return key == 27; // ESC
}

void Calibration::log(){
  // Gets image height and width
  int x = (*original).cols, y = (*original).rows;
  // Draws a small square with the current color, white if none
  cv::rectangle(result, cv::Point(x-25,y-25), cv::Point(x-5,y-5), scalarColor, CV_FILLED);
  // Adds logText to the image, shows if the current color has been saved
  cv::putText(result, logText, cv::Point(x-220,y-5),cv::FONT_HERSHEY_PLAIN, 1.8, scalarColor, 2);
}

void Calibration::readColor(std::string targetColor) {
  // Reads the file
  std::ifstream file(COLORS_FILE_NAME);
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  // Updates current color & cleans logtext
  logText = "";

  std::string color;
  int minHue, maxHue, minSat, maxSat, minVal, maxVal;
  // Reads each line in values.txt and stops when color == targetColor
  while(file >> color >> minHue >> maxHue >> minSat >> maxSat >> minVal >> maxVal && color != targetColor);
  cv::Mat3b hsv((minHue + maxHue) / 2, (minSat + maxSat) / 2, (minVal + maxVal) / 2);
  cv::Mat3b bgr;
  cv::cvtColor(hsv, bgr, CV_HSV2BGR);

  scalarColor = bgr.at<cv::Vec3b>(0, 0);
}

bool Calibration::saveColor(){
  // Reads the file
  std::ifstream inFile(COLORS_FILE_NAME);
  if(inFile.fail()) {
    std::cout << "File could not be opened\n";
    return false;
  }

  // Format a string to e.g. "0 255 0 255 0 255"
  char temp[50];
  sprintf(temp, "%d %d %d %d %d %d", hueMin, hueMax, satMin, satMax, valMin, valMax);

  std::string line, color, wholeFile;
  // Reads each line in the file
  while(std::getline(inFile, line)){
    std::istringstream iss(line);
    // Isolates the color of each line
    iss >> color;
    // If it matches with current Color, changes the entire line with the formatted values from above
    if(color == currentColor)
      line = currentColor + " " + temp;
    // Adds the found line to the wholeText string
    wholeFile += line + "\n";
  }
  inFile.close();

  // Rewrites the entire file
  std::ofstream outFile(COLORS_FILE_NAME);
  outFile << wholeFile;
  outFile.close();

  // Consoles out the rewritten values
  std::cout << currentColor << " " << temp << std::endl;
}

bool Calibration::saveLimits(){
  if (squareBottomRightCorner.first == -1 || finalDistancePixel.first == -1) {
    return false;
  }
  int squareArea = (squareBottomRightCorner.second - squareUpperLeftCorner.second) * 
   (squareBottomRightCorner.first - squareUpperLeftCorner.first); 
  int minArea = squareArea - MIN_MAX_AREA_RANGE / 2;
  int maxArea = squareArea + MIN_MAX_AREA_RANGE / 2;
  int maxDistance = std::sqrt(
   std::pow(finalDistancePixel.first - initialDistancePixel.first, 2) +
   std::pow(finalDistancePixel.second - initialDistancePixel.second, 2));

  std::string fileData = "minArea " + std::to_string(minArea) + 
   "\nmaxArea " + std::to_string(maxArea) + 
   "\nmaxDistance " + std::to_string(maxDistance);

  // Rewrites the entire file
  std::ofstream outFile(LIMITS_FILE_NAME);
  outFile << fileData;
  outFile.close();
}

void Calibration::useClickValues() {
  int pixel_data[3] = {0, 0, 0};
  for (int i = 1; i <= FILTER_SAMPLE; i++) {
      pixel_data[0] += pixels[pixels.size() - i][0];
      pixel_data[1] += pixels[pixels.size() - i][1];
      pixel_data[2] += pixels[pixels.size() - i][2];
  }
  pixel_data[0] /= FILTER_SAMPLE;
  pixel_data[1] /= FILTER_SAMPLE;
  pixel_data[2] /= FILTER_SAMPLE;
  hueMin = pixel_data[0] - MIN_MAX_HSV_RANGE / 2;
  hueMax = pixel_data[0] + MIN_MAX_HSV_RANGE / 2;
  satMin = pixel_data[1] - MIN_MAX_HSV_RANGE / 2;
  satMax = pixel_data[1] + MIN_MAX_HSV_RANGE / 2;
  valMin = pixel_data[2] - MIN_MAX_HSV_RANGE / 2;
  valMax = pixel_data[2] + MIN_MAX_HSV_RANGE / 2;
  updateTrackbars();
}

void Calibration::drawLimits() {
  if (squareBottomRightCorner.first != -1) {
    cv::rectangle(result,
     cv::Point(squareUpperLeftCorner.first, squareUpperLeftCorner.second),
     cv::Point(squareBottomRightCorner.first, squareBottomRightCorner.second), 
     cv::Scalar(55, 175, 212));
  }
  if (finalDistancePixel.first != -1) {
    cv::line(result,
     cv::Point(initialDistancePixel.first, initialDistancePixel.second),
     cv::Point(finalDistancePixel.first, finalDistancePixel.second), 
     cv::Scalar(55, 175, 212));
  }
}

void Calibration::update(){
  // Sets result image as an empty image with original size and type
  result = cv::Mat::zeros((*original).size(), (*original).type());
  // Updates the hsv_image
  cv::cvtColor(*original, hsv_image, cv::COLOR_BGR2HSV);
  if (pixels.size() >= FILTER_SAMPLE) {
    useClickValues();
  }
  // Updates mask values with the corresponding H,S,V
  cv::inRange(hsv_image, cv::Scalar(hueMin, satMin, valMin), cv::Scalar(hueMax, satMax, valMax), mask);
  // Dilates the mask
  cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1));
  // Combines the mask with the original image, as result we get only the filtered colors 
  (*original).copyTo(result, mask);
  // Adds feedback to the window
  log();
  if (getSquareLimits || getDistanceLimits) {
    drawLimits();
  }
  // Displays the resulting image
  cv::imshow(screenName, result);
}