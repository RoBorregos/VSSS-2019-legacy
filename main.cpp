#include "libraries"
#include "libraries/packet/packet.h"
#include "libraries/serial_transmitt/serial_transmitt.h"
#include "vision/Vision.h"

// Validates media input
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
  // ---------------------- VARIABLES --------------------------

  // General
  std::vector<Shape> allies(3), enemies(3);
  Shape ball;

  // Vision
  Vision vision;
  cv::Mat image;
  cv::VideoCapture cap;
  std::string teamColor;
  float circleMinArea = 0.03, circleMaxArea = 0.08, maxHyp = 40;
  bool stat;

  // Serial Transmitt
  SerialTransmitt serialTransmitt;
  Packet packet;

  // ---------------------- SETUP --------------------------
  // General 
  std::cout << std::fixed;
  std::cout << std::setprecision(3);
  std::cout << "Define team color (blue/yellow):\n";
  std::cin >> teamColor;
  teamColor = teamColor == "yellow" ? teamColor : "blue";
  std::cout << "Team color is " << teamColor << "!\n\n";

  // Vision
  stat = inputValidation(argc, argv, image, cap);
  if(!image.data){
    std::cout <<  "Could not open or find the image/video\n";
    return -1;
  }
  vision = Vision(image, teamColor, allies, enemies, ball);
  vision.settings(circleMinArea, circleMaxArea, maxHyp);
  cv::namedWindow("image", cv::WINDOW_AUTOSIZE );

  // Communication
  if (!serialTransmitt.Configure("/dev/ttyUSB0")) {
      std::cout << "Check Port!" << std::endl;
      return -1;
  }

  // ---------------------- CALIBRATION --------------------------




  // ---------------------- MAIN LOOP ------------------------
  while(true){
    // Vision
    cap >> image;
    cv::imshow("image", image);
    vision.update();

    // Strategy


    // Pathplanning



    // Control


    // Communication
    packet.RightMotor(100, 1); // demo
    packet.LeftMotor(150, 0, 0); // demo
    packet.SetId(GOALIE); // demo

    serialTransmitt.SendPacket(packet.GetPacket());

    if(cv::waitKey(30) == 27)
      break;
  }

  std::cout << "Program has ended!" << std::endl;
}