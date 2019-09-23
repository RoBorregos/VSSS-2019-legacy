#include "Vision.h"

Vision::Vision(cv::Mat &image, std::string teamColor, std::vector<Robot> &allies, std::vector<Robot> &enemies, Figure &ball){
  original = &image;

  width = (*original).cols;
  height = (*original).rows;

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
