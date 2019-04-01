#include <opencv2/imgproc.hpp>  // hsv
#include "Vision.h"

// Validates input 
bool isNotValid(int &argc, char** &argv, cv::Mat &image){
  if( argc != 2){
    std::cout <<"Usage: ./main test.png" << std::endl;
    return true;
  }
  
  // Read the file
  std::string path = "images/" + std::string(argv[1]);
  image = cv::imread(path, CV_LOAD_IMAGE_COLOR);

  // Check for invalid input
  if(! image.data ){                             
    std::cout <<  "Could not open or find the image" << std::endl ;
    return true;
  }

  return false;
}

int main(int argc, char** argv){
  cv::Mat image;
  double circleMinArea = 0.03, circleMaxArea = 0.05, maxHyp = 40;

  // Validates the input image
  if(isNotValid(argc, argv, image))
    return -1;
  // cv::resize(image, image, cv::Size(), 0.46, 0.46);

  Vision(image, "blue");
  Vision::settings(circleMinArea, circleMaxArea, maxHyp);

  Vision::update();
}
