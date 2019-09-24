#include "packet.h"

#include <iostream>

bool CheckRightSpeed() {
    Packet packet;

    packet.SetRightSpeed(255);
    if(packet.GetPacket() != 255) {
        return false;
    }
    packet.SetRightSpeed(100);
    if(packet.GetPacket() != 100) {
        return false;
    }
    packet.SetRightSpeed(0);
    if(packet.GetPacket() != 0) {
        return false;
    }
    packet.SetRightSpeed(50);
    if(packet.GetPacket() != 50) {
        return false;
    }
    packet.SetRightSpeed(123);
    if(packet.GetPacket() != 123) {
        return false;
    }
    return true;
}

bool CheckLeftSpeed() {
    Packet packet;

    packet.SetLeftSpeed(255);
    if(packet.GetPacket() != 65280) {
        return false;
    }
    packet.SetLeftSpeed(100);
    if(packet.GetPacket() != 25600) {
        return false;
    }
    packet.SetLeftSpeed(0);
    if(packet.GetPacket() != 0) {
        return false;
    }
    packet.SetLeftSpeed(50);
    if(packet.GetPacket() != 12800) {
        return false;
    }
    packet.SetLeftSpeed(123);
    if(packet.GetPacket() != 31488) {
        return false;
    }

    return true;
}

bool CheckForwardRight() {
    Packet packet;

    packet.SetForwardRight();
    if(packet.GetPacket() != 65536) {
        return false;
    }
    packet.ClearForwardRight();
    if(packet.GetPacket() != 0) {
        return false;
    }
    packet.SetForwardRight();
    if(packet.GetPacket() != 65536) {
        return false;
    }

    return true;
}

bool CheckForwardLeft() {
    Packet packet;

    packet.SetForwardLeft();
    if(packet.GetPacket() != 131072) {
        return false;
    }
    packet.ClearForwardLeft();
    if(packet.GetPacket() != 0) {
        return false;
    }
    packet.SetForwardLeft();
    if(packet.GetPacket() != 131072) {
        return false;
    }

    return true;
}

bool CheckEnableRight() {
    Packet packet;

    packet.SetEnableRight();
    if (packet.GetPacket() != 262144) {
        return false;
    }
    packet.ClearEnableRight();
    if (packet.GetPacket() != 0) {
        return false;
    }
    packet.SetEnableRight();
    if (packet.GetPacket() != 262144) {
        return false;
    }

    return true;
}

bool CheckStop() {
    Packet packet;

    packet.SetStop();
    if (packet.GetPacket() != 1048576) {
        return false;
    }
    packet.ClearStop();
    if (packet.GetPacket() != 0) {
        return false;
    }
    packet.SetStop();
    if (packet.GetPacket() != 1048576) {
        return false;
    }

    return true;
}

bool CheckEnableLeft() {
    Packet packet;

    packet.SetEnableLeft();
    if (packet.GetPacket() != 524288) {
        return false;
    }
    packet.ClearEnableLeft();
    if (packet.GetPacket() != 0) {
        return false;
    }
    packet.SetEnableLeft();
    if (packet.GetPacket() != 524288) {
        return false;
    }

    return true;
}

bool CheckId() {
    Packet packet;

    packet.SetId(GOALIE);
    if (packet.GetPacket() != 0) {
        return false;
    }
    packet.SetId(DEFENDER);
    if (packet.GetPacket() != 2097152) {
        return false;
    }
    packet.SetId(MIDFIELDER);
    if (packet.GetPacket() != 4194304) {
        return false;
    }

    return true;
}

bool CheckPacket() {
    Packet packet;

    packet.SetId(GOALIE);
    packet.SetRightSpeed(164);
    packet.SetForwardRight();
    packet.SetEnableRight();
    if (packet.GetPacket() != 327844) {
        return false;
    }
    packet.SetId(DEFENDER);
    packet.SetLeftSpeed(69);
    packet.SetForwardLeft();
    packet.SetEnableLeft();
    if (packet.GetPacket() != 3098020) {
        return false;
    }

    packet.SetId(MIDFIELDER);
    packet.RightMotor(200, 1);
    packet.LeftMotor(22, 0, 0);
    if (packet.GetPacket() != 4527816) {
        return false;
    }
    
    return true;
}

int main() {
    if(CheckRightSpeed()) {
        std::cout << "OK - RightSpeedCheck" << std::endl;
    } else {
        std::cout << "FAIL - RightSpeedCheck" << std::endl;
    }
    if(CheckLeftSpeed()) {
        std::cout << "OK - LeftSpeedCheck" << std::endl;
    } else {
        std::cout << "FAIL - LeftSpeedCheck" << std::endl;
    }
    if(CheckForwardRight()) {
        std::cout << "OK - ForwardRightCheck" << std::endl;
    } else {
        std::cout << "FAIL - ForwardRightCheck" << std::endl;
    }
    if(CheckForwardLeft()) {
        std::cout << "OK - ForwardLeftCheck" << std::endl;
    } else {
        std::cout << "FAIL - ForwardLeftCheck" << std::endl;
    }
    if(CheckEnableRight()) {
        std::cout << "OK - EnableRightCheck" << std::endl;
    } else {
        std::cout << "FAIL - EnableRightCheck" << std::endl;
    }
    if(CheckEnableLeft()) {
        std::cout << "OK - EnableLeftCheck" << std::endl;
    } else {
        std::cout << "FAIL - EnableLeftCheck" << std::endl;
    }
    if(CheckStop()) {
        std::cout << "OK - StopCheck" << std::endl;
    } else {
        std::cout << "FAIL - StopCheck" << std::endl;
    }
    if (CheckId()) {
        std::cout << "OK - IdCheck" << std::endl;
    } else {
        std::cout << "FAIL - IdCheck" << std::endl;
    }
    if (CheckPacket()) {
        std::cout << "OK - PacketCheck" << std::endl;
    } else {
        std::cout << "FAIL - PacketCheck" << std::endl;
    }

    return 0;
}