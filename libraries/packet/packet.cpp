#include "packet.h"

constexpr uint32_t kLeftSpeedInitialBit = 8;
constexpr uint32_t kForwardRightInitialBit = 16;
constexpr uint32_t kForwardLeftInitialBit = 17;
constexpr uint32_t kEnableRightInitialBit = 18;
constexpr uint32_t kEnableLeftInitialBit = 19;
constexpr uint32_t kStopInitialBit = 20;
constexpr uint32_t kIdInitialBit = 21;

void Packet::SetRightSpeed(unsigned char right_speed) {
    packet_ = (packet_ & 0xFFFFFF00) | right_speed;
}

void Packet::SetLeftSpeed(unsigned char left_speed) {
    packet_ = (packet_ & 0xFFFF00FF) | (left_speed << kLeftSpeedInitialBit);
}

void Packet::SetForwardRight() {
    packet_ |= (1 << kForwardRightInitialBit);
}

void Packet::ClearForwardRight() {
    packet_ &= ~(1 << kForwardRightInitialBit);
}

void Packet::SetForwardLeft() {
    packet_ |= (1 << kForwardLeftInitialBit);
}

void Packet::ClearForwardLeft() {
    packet_ &= ~(1 << kForwardLeftInitialBit);
}

void Packet::SetEnableRight() {
    packet_ |= (1 << kEnableRightInitialBit);
}

void Packet::ClearEnableRight() {
    packet_ &= ~(1 << kEnableRightInitialBit);
}

void Packet::SetEnableLeft() {
    packet_ |= (1 << kEnableLeftInitialBit);
}

void Packet::ClearEnableLeft() {
    packet_ &= ~(1 << kEnableLeftInitialBit);
}

void Packet::SetStop() {
    packet_ |= (1 << kStopInitialBit);
}

void Packet::ClearStop() {
    packet_ &= ~(1 << kStopInitialBit);
}

void Packet::SetId(Id id) {
    packet_ &= ~(7 << kIdInitialBit);
    packet_ |= (id << kIdInitialBit);
}

void Packet::ClearPacket() {
    packet_ = 0;
}

uint32_t Packet::GetPacket() {
    return packet_;
}

void Packet::RightMotor(unsigned char speed, bool forward, bool enable) {
    SetRightSpeed(speed);
    if (forward){
        SetForwardRight();
    } else {
        ClearForwardRight();
    }
    if (enable) {
        SetEnableRight();
    } else {
        ClearEnableRight();
    }
    ClearStop();
}

void Packet::LeftMotor(unsigned char speed, bool forward, bool enable) {
    SetLeftSpeed(speed);
    if (forward){
        SetForwardLeft();
    } else {
        ClearForwardLeft();
    }
    if (enable) {
        SetEnableLeft();
    } else {
        ClearEnableLeft();
    }
    ClearStop();
}