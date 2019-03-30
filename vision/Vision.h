#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

struct foo{
  double x, y, dy, dx, ori;
  bool isFound;
};

struct hsv{
  int hMin, hMax, sMin, sMax, vMin, vMax;
};

class Vision{
  public:
    Vision(cv::Mat &image, std::string teamColor);
    static std::vector<std::vector<cv::Point> > getContours();

  private:
    static void setHSV(hsv &hsvColor, std::string color);
    static void updateMask(hsv hsvColor);

    static cv::Mat *original, masked;
    static foo rob1, rob2, rob3, enm1, enm2, enm3, ball;
    static hsv orange, blue, yellow, red, green, pink, *teamColor;
};