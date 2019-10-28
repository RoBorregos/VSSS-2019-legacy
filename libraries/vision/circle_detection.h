#ifndef CIRCLE_DETECTION_H
#define CIRCLE_DETECTION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <utility>
#include <vector>

class CircleDetection {
  public:
    CircleDetection() {};
    
    // Not copyable or movable
    CircleDetection(const CircleDetection&) = delete;
    CircleDetection& operator=(const CircleDetection&) = delete;

    void loadImage(const cv::Mat& image);
    void setMinM00(const int minM00) {
        this->minM00 = minM00;
    }
    std::vector<std::pair<cv::Mat, cv::Moments>> circlesSegmentation(
      const std::string& hsvFileName);

  private:
    cv::Mat filterByColor(const std::string& hsvFileNam);
    std::vector<std::pair<cv::Mat, cv::Moments>> selectiveSegmentation(cv::Mat& image);
    cv::Moments expandColor(const int row, const int col, cv::Mat& image,
     cv::Mat& object);

    cv::Mat image;
    int minM00 = 0;
};

#endif