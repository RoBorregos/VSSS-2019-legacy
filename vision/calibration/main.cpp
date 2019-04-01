#include <opencv2/imgproc.hpp>  // hsv
#include "Calibration.h"

// Validates input 
bool isNotValid(int &argc, char** &argv, cv::Mat &image){
  if( argc != 2){
    std::cout <<"Usage: ./main test.png" << std::endl;
    return true;
  }
  // Read the file
  std::string path = "../images/" + std::string(argv[1]);
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

  // Validates the input image
  if(isNotValid(argc, argv, image))
    return -1;
  // cv::resize(image, image, cv::Size(), 0.46, 0.46);

  // Initialize video and stores it in 'image'
  // cv::VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
  // cap >> image;

  // Initialize calibration
  Calibration("Calibration", image);

  while(true){
    // Updates the image taken by the camera
    // cap >> image;
    // cv::resize(image, image, cv::Size(), 0.6, 0.6);

    // Exit program if no image was found
    if(image.empty()){
      std::cout << "No image has been found\n";
      break;
    }

    // Updates the resulting image
    Calibration::update();

    // Reads an input key
    if(Calibration::listenKey())
      break;
  }
  return 0;
}