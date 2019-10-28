#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <utility>
#include <vector>

#include "libraries/vision/circle_detection.h"
#include "libraries/packet/packet.h"
#include "libraries/serial_transmitt/serial_transmitt.h"

int main(int argc, char** argv) {
    const cv::Mat bgrImage = cv::imread(
      "libraries/vision/templates_images/robot.jpg", cv::IMREAD_COLOR);

    CircleDetection yellowCircles;
    yellowCircles.loadImage(bgrImage);
    yellowCircles.setMinM00(100000);

    auto yellowCirclesWithMoments = yellowCircles.circlesSegmentation(
      "libraries/vision/color_limits/yellow_limits.txt");


    for (auto yellowCircle : yellowCirclesWithMoments) {
        cv::imshow("yellowCircle", yellowCircle.first);
        cv::waitKey(0);
    }

    SerialTransmitt serialTransmitt;
    // Check the port with ls on /dev
    if (!serialTransmitt.Configure("/dev/ttyUSB0")) {
        std::cout << "Check Port!" << std::endl;
        return -1;
    }
    
    if (yellowCirclesWithMoments.size() > 0) {
        Packet packet;
        packet.RightMotor(100, 1);
        packet.LeftMotor(150, 0, 0);
        packet.SetId(GOALIE);

        serialTransmitt.SendPacket(packet.GetPacket());
    }

    cv::destroyAllWindows();
    return 0;
}