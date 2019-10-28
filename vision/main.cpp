#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <utility>
#include <vector>
#include <queue>

#define BLACK cv::Vec3b(0, 0, 0)
#define FILTER_SAMPLE 10
#define MIN_M00 100000
#define WHITE cv::Vec3b(255, 255, 255)

std::vector<cv::Vec3b> pixels;

void mouseEvent(int event, int x, int y, int flags, void* param) {
  cv::Mat* image = (cv::Mat*)param;

  switch (event) {
      case CV_EVENT_LBUTTONDOWN:
          pixels.push_back(image->at<cv::Vec3b>(y,x));
          std::cout << x << "\t" << y << std::endl;
          break;
      case CV_EVENT_MOUSEMOVE:
          break;
      case CV_EVENT_LBUTTONUP:
          break;
  }
}

///////////////////////////////
// Calibration of the colors //
///////////////////////////////

// std::vector<int> filterHSV(const cv::Mat& hsvImage) {
//     std::vector<int> limits {0, 0, 0, 0, 0, 0};
//     int data[3] = {0, 0, 0};
//     cv::Mat filteredImage;

//     while (cv::waitKey(1) != 'x') {  
//         if (pixels.size() >= FILTER_SAMPLE) {
//         for (int i = 0; i < FILTER_SAMPLE; i++) {
//             data[0] += pixels[pixels.size() - i][0];
//             data[1] += pixels[pixels.size() - i][1];
//             data[2] += pixels[pixels.size() - i][2];
//         }
//         data[0] /= FILTER_SAMPLE;
//         data[1] /= FILTER_SAMPLE;
//         data[2] /= FILTER_SAMPLE;
//         int limit = 0;
//         for (int i = 0; i < 3; i++) {
//             limits[limit] = data[i] - 50;
//             limits[limit + 1] = data[i] + 50;
//             limit += 2;
//         }
//         cv::inRange(hsvImage, cv::Scalar(limits[0], limits[2], limits[4]),
//             cv::Scalar(limits[1], limits[3], limits[5]), filteredImage);
//         cv::imshow("HSV Filtered Image", filteredImage);
//         }

//     }
//     pixels.clear();
//     cv::destroyWindow("HSV Filtered Image");
//     return limits;
// }

// void getHSVTresholds(std::string fileName, cv::Mat hsvImage) {
//     cv::imshow("HSV Image", hsvImage);
//     cv::setMouseCallback("HSV Image", mouseEvent, &hsvImage);
//     auto limits = filterHSV(hsvImage);

//     std::ofstream output_file(fileName);
//     for (auto limit : limits) {
//         output_file << limit << std::endl;
//     }
//     cv::destroyWindow("HSV Image");
// }

// void getThresholds(const cv::Mat bgrImage) {
//     cv::Mat hsvImage;
//     cv::cvtColor(bgrImage, hsvImage, cv::COLOR_BGR2HSV);
//     getHSVTresholds("yellow_limits.txt", hsvImage); 
//     getHSVTresholds("green_limits.txt", hsvImage);
// }

cv::Mat filterByColor(const cv::Mat& bgrImage, const std::string& hsvFileName) {
    std::vector<int> limits;
    std::ifstream hsvLimisFile(hsvFileName);
    std::string line;
    while (std::getline(hsvLimisFile, line)) {
      limits.push_back(std::stoi(line));
    }

    cv::Mat hsvImage;
    bgrImage.copyTo(hsvImage);
    cv:erode(hsvImage, hsvImage, cv::Mat());
    cv::cvtColor(hsvImage, hsvImage, cv::COLOR_BGR2HSV);

    // Binaryze the image with the given limits, returns a 1 channel image.
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(limits[0], limits[2], limits[4]),
      cv::Scalar(limits[1], limits[3], limits[5]), mask);
    
    cv::Mat colorShapes(bgrImage.rows, bgrImage.cols, CV_8UC3, cv::Scalar(0,0,0));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(bgrImage, mask, colorShapes);
    return colorShapes;
}

cv::Moments expandColor(const int row, const int col, cv::Mat& image, cv::Mat& object) {
  object = cv::Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
  std::queue<int> queue;
  queue.push(row);
  queue.push(col);

  while(!queue.empty()) {
    int row = queue.front();
    queue.pop();
    int col = queue.front();
    queue.pop();

    object.at<cv::Vec3b>(row, col) = WHITE;
    image.at<cv::Vec3b>(row, col) = BLACK;

    if (row - 1 >= 0) {
      if (image.at<cv::Vec3b>(row - 1, col) == WHITE) {
        image.at<cv::Vec3b>(row - 1, col) = BLACK;
        queue.push(row - 1);
        queue.push(col);
      }
    }
    if (col + 1 < image.cols) {
      if (image.at<cv::Vec3b>(row, col + 1) == WHITE) {
        image.at<cv::Vec3b>(row, col + 1) = BLACK;
        queue.push(row);
        queue.push(col + 1);
      }
    }
    if (col - 1 >= 0) {
      if (image.at<cv::Vec3b>(row, col - 1) == WHITE) {
        image.at<cv::Vec3b>(row, col - 1) = BLACK;
        queue.push(row);
        queue.push(col - 1);
      }
    }
    if (row + 1 < image.rows) {
      if (image.at<cv::Vec3b>(row + 1, col) == WHITE) {
        image.at<cv::Vec3b>(row + 1, col) = BLACK;
        queue.push(row + 1);
        queue.push(col);
      }
    }
  }
  cv::cvtColor(object, object, cv::COLOR_RGB2GRAY);
  return cv::moments(object, false);
}

std::vector<std::pair<cv::Mat, cv::Moments>> selectiveSegmentation(cv::Mat& image) {
    std::vector<std::pair<cv::Mat, cv::Moments>> objects;
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (image.at<cv::Vec3b>(row, col) == WHITE) {
                cv::Mat object;
                cv::Moments moment = expandColor(row, col, image, object);
                if (moment.m00 > MIN_M00) {
                    cv::Mat circleTemplate = cv::imread("images/circle.jpg", cv::IMREAD_COLOR);
                    cv:erode(circleTemplate, circleTemplate, cv::Mat());
                    cv::threshold(circleTemplate, circleTemplate, 0, 255, 0);
                    cv::cvtColor(circleTemplate, circleTemplate, cv::COLOR_BGR2GRAY);
                    if (cv::matchShapes(circleTemplate, object, CV_CONTOURS_MATCH_I2, 0) 
                      < 0.10 ) {
                        objects.push_back(std::make_pair(object, moment));
                    }
                }
            }
        }
    }
    return objects;
}

int main(int argc, char** argv){
    const cv::Mat bgrImage = cv::imread("images/robot.jpg", cv::IMREAD_COLOR);
    cv::imshow("bgrImage", bgrImage);

    auto greenObjects = filterByColor(bgrImage, "color_limits/green_limits.txt");
    auto yellowObjects = filterByColor(bgrImage, "color_limits/yellow_limits.txt");

    cv::threshold(greenObjects, greenObjects, 0, 255, 0);
    auto bynaryGreenCircles = selectiveSegmentation(greenObjects);
    cv::threshold(yellowObjects, yellowObjects, 0, 255, 0);
    auto bynaryYellowCircles = selectiveSegmentation(yellowObjects);

    cv::Mat greenAndYellowCircles(bgrImage.rows, bgrImage.cols, CV_8UC3, cv::Scalar(0,0,0));
    for (auto yellowCircle : bynaryYellowCircles) {
        cv::cvtColor(yellowCircle.first,yellowCircle.first, cv::COLOR_GRAY2BGR);
        cv::bitwise_or(greenAndYellowCircles, yellowCircle.first, 
          greenAndYellowCircles);
        cv::bitwise_and(bgrImage, greenAndYellowCircles, greenAndYellowCircles);
    }
    for (auto greenCircle : bynaryGreenCircles) {
        cv::cvtColor(greenCircle.first,greenCircle.first, cv::COLOR_GRAY2BGR);
        cv::bitwise_or(greenAndYellowCircles, greenCircle.first, 
          greenAndYellowCircles);
        cv::bitwise_and(bgrImage, greenAndYellowCircles, greenAndYellowCircles);
    }
    cv::imshow("greenAndYellowCircles", greenAndYellowCircles);

    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}