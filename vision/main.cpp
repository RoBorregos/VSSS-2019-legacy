#include <opencv2/imgproc.hpp>  // hsv
#include "Vision.h"

// Validates input. True if static media / False if not
bool inputValidation(int &argc, char** &argv, cv::Mat &image, cv::VideoCapture &cap){
  std::string path;
  
  if(argc > 1){
    path = "media/" + std::string(argv[1]);
    if(int(std::string(argv[1]).find(".png")) > -1){
      image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
      return false;
    }
  }

  argc > 1 && std::string(argv[1]).find(".mp4") ? cap.open(path) : cap.open(0);
  cap >> image;
  return true;
}

int main(int argc, char** argv){
  cv::Mat image;
  cv::VideoCapture cap;
  std::string teamColor;
  std::vector<Shape> allies(3), enemies(3);
  Shape ball;
  float circleMinArea = 0.03, circleMaxArea = 0.08, maxHyp = 40;

  std::cout << std::fixed;
  std::cout << std::setprecision(3);

  bool stat = inputValidation(argc, argv, image, cap);
  
  if(!image.data){
    std::cout <<  "Could not open or find the image/video\n";
    return -1;
  }

  std::cout << "Define team color (blue/yellow):\n";
  std::cin >> teamColor;
  teamColor = teamColor == "yellow" ? teamColor : "blue";
  std::cout << "Team color is " << teamColor << "!\n\n";

  Vision vision = Vision(image, teamColor, allies, enemies, ball);
  vision.settings(circleMinArea, circleMaxArea, maxHyp);
  cv::namedWindow("image", cv::WINDOW_AUTOSIZE );

  while(true){
    if(stat)
      cap >> image;
      //cv::resize(image, image, cv::Size(), 0.5, 0.5);
      cv::imshow("image", image);
      vision.update();
      
    // Exit program if no image was found
    if(image.empty()){
      std::cout << "No image has been found\n";
      break;
    }

    if(cv::waitKey(30) == 27)
      break;
  }
  return 0;
}