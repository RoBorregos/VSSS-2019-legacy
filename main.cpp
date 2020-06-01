#include "libraries/packet/packet.h"
#include "libraries/serial_transmitt/serial_transmitt.h"
#include "vision/Vision.h"
#include "libraries/Control/Control.h"

// Validates media input
bool inputValidation(int &argc, char** &argv, cv::Mat &image, cv::VideoCapture &cap){
  std::string path;
  
  if(argc > 1){
    path = "vision/media/" + std::string(argv[1]);
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
  cv::Mat image;
  cv::VideoCapture cap;
  std::string teamColor;
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
  Vision vision = Vision(image, teamColor, allies, enemies, ball);
  vision.setDraw(true);
  // Control
  Control control = Control(allies);

  // Communication
  // if (!serialTransmitt.Configure("/dev/ttyUSB0")) {
  //     std::cout << "Check Port!" << std::endl;
  //     return -1;
  // }

  // ---------------------- CALIBRATION --------------------------




  // ---------------------- MAIN LOOP ------------------------
  while(true){
    // Vision
    cap >> image;
    if(image.empty()){
      std::cout << "No image has been found\n";
    }
    else{
      // cv::imshow("image", image);
      vision.update();
      // vision.show();

      // Strategy


      // Pathplanning

      
      // Control. To this point needs an updated robot current and final position of allies.
      double r=0,l=0;

      for(int i=0; i<3; i++){
        control.move(i,r,l); //returns right and left velocity to go to the robot current position to the final position (closest angle). Never stops. 
        // Communication 
        packet.setId(i);
        packet.RightMotor(fabs(r), r>0, 0);
        packet.LeftMotor(fabs(l), l>0, 0);

        serialTransmitt.SendPacket(packet.GetPacket());

      }

    }

    if(cv::waitKey(30) == 27)
      break;
  }

  std::cout << "Program has ended!" << std::endl;
}