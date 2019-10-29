#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

enum Id{
    GOALIE,
    DEFENDER,
    MIDFIELDER
};

// Packet
// First Byte = Right Motor PWM
// Second Byte = Left Motor PWM
// 17th bit =  Forward RigPacketht
// 18th bit = Forward Left
// 19th bit = Enable Right
// 20th bit = Enable Left
// 21th bit = Stop
// 22 - 24 bit = Id
// 25 - 32 bit = unused
class Packet {
  public:
    Packet() {};
    
    // Not copyable or movable
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    uint32_t GetPacket();

    void SetRightSpeed(unsigned char rightSpeed);
    void SetLeftSpeed(unsigned char leftSpeed);
    void SetStop();
    void ClearStop();
    void SetForwardRight();
    void ClearForwardRight();
    void SetForwardLeft();
    void ClearForwardLeft();
    void SetEnableRight();
    void ClearEnableRight();
    void SetEnableLeft();
    void ClearEnableLeft();
    void SetId(Id id);
    void ClearPacket();

    void RightMotor(unsigned char speed, bool forward, bool enable = true);
    void LeftMotor(unsigned char speed, bool forward, bool enable = true);

  private:
    uint32_t packet_ = 0;
};

#endif