#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv2/highgui/highgui.hpp> // image & screen
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

#define ORANGE "Orange"
#define BLUE "Blue"
#define YELLOW "Yellow"
#define RED "Red"
#define GREEN "Green"
#define PINK "Pink"
#define WHITE "White"

class Calibration{
  public:
    Calibration(std::string screenName, cv::Mat &image);

    // Listen to key inputs, returns true if 'ESC'
    static bool listenKey();
    // Shows an image with just the filtered colors
    static void update();

    static cv::Mat result;
    static std::string screenName;

  private:
    static void useClickValues();
    static void drawLimits();
    static void updateTrackbars();
    // Trackbar callbacks
    static void onHueMinTrackBar(int, void*);
    static void onHueMaxTrackBar(int, void*);
    static void onSatMinTrackBar(int, void*);
    static void onSatMaxTrackBar(int, void*);
    static void onValMinTrackBar(int, void*);
    static void onValMaxTrackBar(int, void*);

    // Shows in the screen the current color and when it's been saved
    static void log(); 
    // Rewrites the current color's hsv range in the values.txt file
    static bool saveColor();
    static bool saveLimits();

    // Updates the trackbars with the values inside the values.txt file of a specific color
    static void readColor(std::string targetColor);

    static int hueMin, hueMax, satMin, satMax, valMin, valMax;
    static cv::Mat hsv_image, mask, *original;
    static std::string logText, currentColor;
    static std::ifstream infile;
    static cv::Scalar scalarColor;
};

#endif //CALIBRATION_H