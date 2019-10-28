#include "circle_detection.h"

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
#define WHITE cv::Vec3b(255, 255, 255)

cv::Mat CircleDetection::filterByColor(const std::string& hsvFileName) {
    std::vector<int> limits;
    std::ifstream hsvLimisFile(hsvFileName);
    std::string line;
    while (std::getline(hsvLimisFile, line)) {
      limits.push_back(std::stoi(line));
    }

    cv::Mat hsvImage;
    image.copyTo(hsvImage);
    cv:erode(hsvImage, hsvImage, cv::Mat());
    cv::cvtColor(hsvImage, hsvImage, cv::COLOR_BGR2HSV);

    // Binaryze the image with the given limits, returns a 1 channel image.
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(limits[0], limits[2], limits[4]),
      cv::Scalar(limits[1], limits[3], limits[5]), mask);
    
    cv::Mat colorShapes(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(image, mask, colorShapes);
    return colorShapes;
}

void CircleDetection::loadImage(const cv::Mat& image) {
    image.copyTo(this->image);
}

cv::Moments CircleDetection::expandColor(const int row, const int col, cv::Mat& image, cv::Mat& object) {
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

std::vector<std::pair<cv::Mat, cv::Moments>> CircleDetection::selectiveSegmentation(
  cv::Mat& image) {
    std::vector<std::pair<cv::Mat, cv::Moments>> objects;
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (image.at<cv::Vec3b>(row, col) == WHITE) {
                cv::Mat object;
                cv::Moments moment = expandColor(row, col, image, object);
                if (moment.m00 > minM00) {
                    cv::Mat circleTemplate = cv::imread("templates_images/circle.jpg", cv::IMREAD_COLOR);
                    // cv::imshow("circleTemplate", circleTemplate);
                    // cv::waitKey(0);
                    // cv:erode(circleTemplate, circleTemplate, cv::Mat());
                    // cv::threshold(circleTemplate, circleTemplate, 0, 255, 0);
                    // cv::cvtColor(circleTemplate, circleTemplate, cv::COLOR_BGR2GRAY);
                    // if (cv::matchShapes(circleTemplate, object, CV_CONTOURS_MATCH_I2, 0) 
                    //   < 0.10 ) {
                        objects.push_back(std::make_pair(object, moment));
                    // }
                }
            }
        }
    }
    return objects;
}

std::vector<std::pair<cv::Mat, cv::Moments>> CircleDetection::circlesSegmentation(
  const std::string& hsvFileName) {
    auto colorObjects = filterByColor(hsvFileName);
    cv::threshold(colorObjects, colorObjects, 0, 255, 0);
    auto binaryCirclesWithMoments = selectiveSegmentation(colorObjects);

    cv::Mat blackImage(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
    for (int i = 0; i < binaryCirclesWithMoments.size(); i++) {
      cv::cvtColor(binaryCirclesWithMoments[i].first, binaryCirclesWithMoments[i].first, cv::COLOR_GRAY2BGR);
      cv::bitwise_or(blackImage, binaryCirclesWithMoments[i].first, blackImage);
      cv::bitwise_and(image, blackImage, blackImage);
      blackImage.copyTo(binaryCirclesWithMoments[i].first);
    }

    return binaryCirclesWithMoments;
}