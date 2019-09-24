#ifndef PACKET_H
#define PACKET_H

#include <climits>
#include <stdint.h>
#include <string>
#include <unistd.h>

enum Id{
    GOALIE,
    DEFENDER,
    MIDFIELDER
};

// Packet
// First Byte = Right Motor PWM
// Second Byte = Left Motor PWM
// 17th bit =  Forward Right
// 18th bit = Forward Left
// 19th bit = Enable Right
// 20th bit = Enable Left
// 21th bit = Stop
// 22 - 24 bit = Id
// 25 - 32 bit = unused
class Packet {
  public:
    Packet() {};
    ~Packet() {
        close(file_descriptor_);
    };
    // Not copyable or movable
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    bool Configure(std::string port);
    bool SendPacket();
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

  private:
    uint32_t packet_ = 0;
    int file_descriptor_ = INT_MIN;
};

#endif