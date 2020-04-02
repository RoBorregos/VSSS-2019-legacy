#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

// Scalar constants
#define ORANGE cv::Scalar(0,165,255)
#define BLUE cv::Scalar(255,0,0)
#define YELLOW cv::Scalar(0,255,255)
#define RED cv::Scalar(0,0,255)
#define GREEN cv::Scalar(0,255,0)
#define PINK cv::Scalar(147,20,255)
#define WHITE cv::Scalar(255,255,255)

class Calibration{
  public:
    Calibration(std::string screenName, cv::Mat &image);
    int listenKey();
    void update();

  private:
    void log();
    void saveColor();
    void readColor(std::string targetColor);

    static void onMouse(int event, int x, int y, int, void *userdata);
    void onMouse(int event, int x, int y);

    int hueMin, hueMax, satMin, satMax, valMin, valMax;
    int epsilon[3];
    cv::Mat hsv_image, mask, *original;
    std::string logText, currentColor;
    std::ifstream infile;
    cv::Scalar scalarColor;
    cv::Mat result;
    std::string screenName;
};