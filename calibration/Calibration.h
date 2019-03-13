#include <opencv2/highgui/highgui.hpp> // image & screen
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <iostream>

class Calibration{
  public:
    Calibration(std::string screenName, cv::Mat &image);

    static void onHueMinTrackBar(int, void*);
    static void onHueMaxTrackBar(int, void*);
    static void onSatMinTrackBar(int, void*);
    static void onSatMaxTrackBar(int, void*);
    static void onValMinTrackBar(int, void*);
    static void onValMaxTrackBar(int, void*);

    static void update();


    static cv::Mat result;
    static std::string screenName; 

  private:
    static int hueMin, hueMax, satMin, satMax, valMin, valMax;
    static cv::Mat hsv_image, mask;
    static cv::Mat *original;
};