#include "Calibration.h"

Calibration::Calibration(std::string screenName, cv::Mat &image){
  // Assigning constructor values
  this -> screenName = screenName;
  original = &image;

  currentColor = "White";
  scalarColor = WHITE;
  logText = "";
  mode = HSV_COLORS;

  isDrawing = false;
  stopDrawing = true;

  // Sets maximum slider values to 255
  hueMax = 255; satMax = 255; valMax = 255;
  // Sets minimum slider values to 0
  hueMin = 0; satMin = 0; valMin = 0;

  // Sets epsilson values
  epsilon[0] = 5;   // H
  epsilon[1] = 90;  // S
  epsilon[2] = 100; // V

  // Creates a window display for current image
  cv::namedWindow(screenName, cv::WINDOW_AUTOSIZE);

  // Creates callback for screenName
  cv::setMouseCallback(screenName, onMouse, this);

  // Stores a hsv copy of the original image as 'hsv_image'
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

  // Creates 6 sliders in our window display with a maxium value of 255
  cv::createTrackbar("Low  H", screenName, &hueMin, 255);
  cv::createTrackbar("High H", screenName, &hueMax, 255);
  cv::createTrackbar("Low  S", screenName, &satMin, 255);
  cv::createTrackbar("High S", screenName, &satMax, 255);
  cv::createTrackbar("Low  V", screenName, &valMin, 255);
  cv::createTrackbar("High V", screenName, &valMax, 255);

  // Displays the original image with the sliders, once
  update();
}

int Calibration::listenKey(){
  char key = (char) cv::waitKey(30);
  switch(key){
    case '1':
      mode = HSV_COLORS;
      stopDrawing = true;
      logText = "";
      break;
    case '2':
      mode = INITIAL_POINT;
      stopDrawing = true;
      logText = "";
      break;
    case '3':
      mode = AREA_LIMITS;
      stopDrawing = true;
      logText = "";
      break;
    case '4':
      mode = DISTANCES;
      stopDrawing = true;
      logText = "";
      break;
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
    case 'c': // Keep currentColor but changes color to DEFAULT
      readColor("DEFAULT");
      break;
    case 10: // ENTER
      if(scalarColor != WHITE){
        logText = "Color saved!";
        saveColor();
      }
      break;
    case 32: // SPACE
      return 0;
    case 27: // ESC
      return -1;
  }
  return 1;
}

void Calibration::log(){
  // Gets image height and width
  int x = (*original).cols, y = (*original).rows;
  // Draws a small square with the current color, white if none
  cv::rectangle(result, cv::Point(x-25,y-25), cv::Point(x-5,y-5), scalarColor, CV_FILLED);
  // Adds logText to the image, shows if the current color has been saved
  if(mode == DISTANCES)
    cv::putText(result, logText, cv::Point(x-130,y-5),cv::FONT_HERSHEY_PLAIN, 1.2, RED, 1);
  else
    cv::putText(result, logText, cv::Point(x-220,y-5),cv::FONT_HERSHEY_PLAIN, 1.8, scalarColor, 2);

  std::string modeText;
  switch(mode){
    case HSV_COLORS:
      modeText = "Set HSV colors";
      break;
    case INITIAL_POINT:
      modeText = "Set initial point";
      break;
    case AREA_LIMITS:
      modeText = "Grab area limits";
      break;
    case DISTANCES:
      modeText = "Get distances";
      break;
    default:
      modeText = "";
  }
  cv::putText(result, modeText, cv::Point(5,y-5),cv::FONT_HERSHEY_PLAIN, 1.2, RED, 2);
}

void Calibration::saveColor(){
  // Reads the file
  std::ifstream inFile("colors.txt");
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
  std::ofstream outFile("colors.txt");
  outFile << wholeFile;
  outFile.close();

  // Consoles out the rewritten values
  std::cout << currentColor << " " << temp << std::endl;
}

void Calibration::readColor(std::string targetColor){
  // Reads the file
  std::ifstream file("colors.txt");
  if(file.fail()){
    std::cout << "File could not be opened\n";
    return;  
  }

  // Updates current color & cleans logtext
  currentColor = targetColor != "DEFAULT" ? targetColor : currentColor;
  logText = "";

  std::string color;

  // Reads each line in colors.txt and stops when color == targetColor
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
  
  // Draw lines from mouseEvents
  if(!stopDrawing)
    cv::line(result, initialP, finalP, RED, 1);

  // Adds feedback to the window
  log();
  // Displays the resulting image
  cv::imshow(screenName, result);
}

void Calibration::onMouse(int event, int x, int y, int, void *userdata){
  Calibration *calibration = reinterpret_cast<Calibration *>(userdata);
  calibration->onMouse(event, x, y);
}

void Calibration::onMouse(int event, int x, int y){
  switch(mode){
    case HSV_COLORS:
      if(event == CV_EVENT_LBUTTONDOWN){
        cv::Vec3b HSV_color = (hsv_image).at<cv::Vec3b>(cv::Point(x, y));

        std::cout << "H = " << (int)HSV_color[0] << std::endl;
        std::cout << "S = " << (int)HSV_color[1] << std::endl;
        std::cout << "V = " << (int)HSV_color[2] << std::endl;

        hueMin = std::max(HSV_color[0] - epsilon[0], 0);
        satMin = std::max(HSV_color[1] - epsilon[1], 0);
        valMin = std::max(HSV_color[2] - epsilon[2], 0);
        hueMax = std::min(HSV_color[0] + epsilon[0], 255);
        satMax = std::min(HSV_color[1] + epsilon[1], 255);
        valMax = std::min(HSV_color[2] + epsilon[2], 255);

        cv::setTrackbarPos("Low  H", screenName, hueMin);
        cv::setTrackbarPos("High H", screenName, hueMax);
        cv::setTrackbarPos("Low  S", screenName, satMin);
        cv::setTrackbarPos("High S", screenName, satMax);
        cv::setTrackbarPos("Low  V", screenName, valMin);
        cv::setTrackbarPos("High V", screenName, valMax);
      }
      break;
    
    case DISTANCES:
      switch(event){
        case CV_EVENT_LBUTTONDOWN:
          initialP = finalP = cv::Point(x,y);
          isDrawing = true;
          stopDrawing = false;
          break;
        case CV_EVENT_LBUTTONUP:
          finalP = cv::Point(x,y);
          isDrawing = false;
          char str[10];
          sprintf(str, "%.2f px", cv::norm(finalP - initialP));
          logText = str;
          break;
        case CV_EVENT_MOUSEMOVE:
          if(isDrawing) finalP = cv::Point(x,y);
      }
      break;
  }
  
  
}