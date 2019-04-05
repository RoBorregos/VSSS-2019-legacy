#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <iomanip>

#define PI 3.14159265

struct foo{
  double x, y, dy, dx, ori;
  bool firstTimeFlag = true;
  std::chrono::time_point<std::chrono::system_clock> lastTime;
};

struct hsv{
  int hMin, hMax, sMin, sMax, vMin, vMax;
};

struct c_pair{
  cv::Point2f c_color, c_teamColor;
  c_pair(cv::Point2f c_color, cv::Point2f c_team){
    this -> c_color = c_color;
    this -> c_teamColor = c_team;
  }
};

class Vision{
  public:
    Vision(cv::Mat &image, std::string teamColor);
    static void settings(double &circleMinArea, double &circleMaxArea, double &maxHyp);
    static void update();


  private:
    static void setHSV(hsv &hsvColor, std::string color);
    static void updateMask(hsv hsvColor);
    static std::vector<std::vector<cv::Point> > getContours(hsv color);
    static void fixContours(std::vector<std::vector<cv::Point> > &contours);
    static void drawContours(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Point2f> centroids);
    static std::vector<cv::Point2f> getCentroids(hsv color);
    static c_pair getCentroidPair(std::vector<cv::Point2f> c_color, std::vector<cv::Point2f> c_target);
    static void updateValues(foo &f, c_pair cp);

    static cv::Mat *original, masked;
    static foo rob1, rob2, rob3, enm1, enm2, enm3, ball;
    static hsv orange, blue, yellow, red, green, pink;
    static double width, height, *circleMinArea, *circleMaxArea, *maxHyp;
    static std::vector<cv::Vec4i> hierarchy;
    static std::string teamColor;
};
