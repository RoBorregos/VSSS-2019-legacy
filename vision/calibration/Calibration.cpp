#include "Calibration.h"

// Static variables declaration
cv::Scalar Calibration::scalarColor;
std::ifstream infile;
std::string Calibration::screenName, Calibration::logText, Calibration::currentColor;
cv::Mat Calibration::hsv_image, Calibration::mask, Calibration::result;
cv::Mat *Calibration::original;
int Calibration::hueMin, Calibration::hueMax, Calibration::satMin, Calibration::satMax,
    Calibration::valMin, Calibration::valMax;

Calibration::Calibration(std::string screenName, cv::Mat &image){
  // Assigning constructor values
  this -> screenName = screenName;
  original = &image;

  currentColor = "White";
  scalarColor = WHITE;
  logText = "";

  // Sets maximum slider values to 255
  hueMax = 255; satMax = 255; valMax = 255;
  
  // Creates a window display with 'screenName' as name
  cv::namedWindow(screenName, cv::WINDOW_AUTOSIZE);
  // Stores a hsv copy of the original image as 'hsv_image'
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

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

bool Calibration::listenKey(){
  char key = (char) cv::waitKey(30);

  switch(key){
    case 'o':
      scalarColor = ORANGE;
      readColor("Orange");
      break;
    case 'b':
      scalarColor = BLUE;
      readColor("Blue");
      break;
    case 'y':
      scalarColor = YELLOW;
      readColor("Yellow");
      break;
    case 'r':
      scalarColor = RED;
      readColor("Red");
      break;
    case 'g':
      scalarColor = GREEN;
      readColor("Green");
      break;
    case 'p':
      scalarColor = PINK;
      readColor("Pink");
      break;
    case 13: // ENTER
      if(scalarColor != WHITE){
        logText = "Color saved!";
        saveColor();
      }
      break;
    default:
      if(key != -1){
        readColor("");
        scalarColor = WHITE;
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

void Calibration::readColor(std::string targetColor){
  // Reads the file
  std::ifstream file("Values.txt");
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  // Updates current color & cleans logtext
  currentColor = targetColor;
  logText = "";

  std::string color;

  // Reads each line in values.txt and stops when color == targetColor
  while(file >> color >> hueMin >> hueMax >> satMin >> satMax >> valMin >> valMax && color != targetColor);

  // Rewrites the trackbar values with the found ones in the file. 
  // If the color was not found, it'll be setted to the DEFAULT values found at the end of the file
  cv::setTrackbarPos("Low  H", screenName, hueMin);
  cv::setTrackbarPos("High H", screenName, hueMax);
  cv::setTrackbarPos("Low  S", screenName, satMin);
  cv::setTrackbarPos("High S", screenName, satMax);
  cv::setTrackbarPos("Low  V", screenName, valMin);
  cv::setTrackbarPos("High V", screenName, valMax);
}

void Calibration::saveColor(){
  // Reads the file
  std::ifstream inFile("Values.txt");
  if(inFile.fail()){
    std::cout << "File could not be opened\n";
    return;
  }

  // Format a string to e.g. "0 255 0 255 0 255"
  char temp[50];
  sprintf(temp, "%d %d %d %d %d %d",hueMin,hueMax,satMin,satMax,valMin,valMax);

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
  std::ofstream outFile("Values.txt");
  outFile << wholeFile;
  outFile.close();

  // Consoles out the rewritten values
  std::cout << currentColor << " " << temp << std::endl;
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
  // Adds feedback to the window
  log();
  // Displays the resulting image
  cv::imshow(screenName, result);
}