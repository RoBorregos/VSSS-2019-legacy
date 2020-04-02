#include <opencv2/imgproc.hpp>  // hsv
#include "Calibration.h"

// Validates input. True if static media / False if not
bool inputValidation(int argc, char** argv, cv::Mat &image, cv::VideoCapture &cap){
  std::string path;
  
  if(argc > 1){
    path = "../media/" + std::string(argv[1]);
    if(int(std::string(argv[1]).find(".png")) > -1){
      image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
      return true;
    }
  }

  argc > 1 && std::string(argv[1]).find(".mp4") ? cap.open(path) : cap.open(0);
  cap >> image;
  return false;
}

int main(int argc, char** argv){
  std::cout <<"Usage(static video): ./main test.mp4" << std::endl;
  std::cout <<"Usage(static image): ./main test.png" << std::endl;
  std::cout <<"Usage(video capture): ./main" << std::endl;

  cv::Mat image;
  cv::VideoCapture cap;
  bool isStatic = inputValidation(argc, argv, image, cap);
  int key;

  if(!image.data){
    std::cout <<  "Could not open or find the image/video" << std::endl ;
    return -1;
  }

  // Initialize calibration
  Calibration calibration("Calibration", image);

  while(true){
    // Updates the image taken by the camera
    if(!isStatic)
      cap >> image;

    // Exit program if no image was found
    if(image.empty()){
      std::cout << "No image has been found\n";
      break;
    }

    // Reads an input key
    key = calibration.listenKey();
    if(key == 0) // SPACE BAR
      isStatic = !isStatic;
    else if(key == -1) // ESC
      break;

    // Updates the resulting image
    calibration.update();
  }

  return 0;
}