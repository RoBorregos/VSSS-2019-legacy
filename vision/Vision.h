#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <iomanip>
#include "../libraries/Shape/Shape.h"

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

  c_pair(){
    c_color = cv::Point2f(0,0);
    c_teamColor = cv::Point2f(0,0);
  }
};

class Vision{
  public:
    Vision(cv::Mat &image, std::string teamColor, std::vector<Shape> &allies, std::vector<Shape> &enemies, Shape &ball);
    void update();
    void setDraw(bool b);

  private:
    void setCrossPoints();
    void setLimits();
    void setHSV(hsv &hsvColor, std::string color);
    void updateMask(hsv hsvColor);
    std::vector<std::vector<cv::Point> > getContours(hsv color);
    void fixContours(std::vector<std::vector<cv::Point> > &contours);
    void drawContours(std::vector<std::vector<cv::Point> > contours, std::vector<cv::Point2f> centroids);
    std::vector<cv::Point2f> getCentroids(hsv color);
    c_pair getCentroidPair(std::vector<cv::Point2f> c_color, std::vector<cv::Point2f> c_target);
    void updateValues(Shape &f, c_pair cp);
    bool centroidIsEmpty(c_pair cp);
    void draw(Point ref, c_pair cp, float ori);
    Point getRealPos(float x, float y);

    std::vector<Point> crossPoints;
    std::vector<Shape> allies;
    std::vector<Shape> enemies;
    Shape ball;

    cv::Mat *original, masked, drawnImg;
    float width, height, minArea, maxArea, maxDistance, scale;
    std::string teamColor;
    hsv orange, blue, yellow, red, green, pink;
    std::vector<cv::Vec4i> hierarchy;
    bool drawSet;
};
