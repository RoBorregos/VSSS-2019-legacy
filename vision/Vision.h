#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <iomanip>
#include "../libraries/Robot/Robot.h"

#define PI 3.14159265

struct hsv{
  int hMin, hMax, sMin, sMax, vMin, vMax;
};

struct c_pair{
  cv::Point2f c_color, c_teamColor;
  c_pair(cv::Point2f color, cv::Point2f teamColor){
    c_color = color;
    c_teamColor = teamColor;
  }
};

class Vision{
  public:
    Vision(cv::Mat &image, std::string teamColor, std::vector<Robot> &allies, std::vector<Robot> &enemies, Figure &ball);

  private:
    void setHSV(hsv &hsvColor, std::string color);

    std::vector<Robot> allies;
    std::vector<Robot> enemies;
    Figure ball;

    cv::Mat *original, masked;
    float width, height;
    hsv orange, blue, yellow, red, green, pink;
    std::string teamColor;
};
