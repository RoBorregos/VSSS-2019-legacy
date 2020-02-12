//  RoBorregos
//  Tecnológico de Monterrey
//  Monterrey, Nuevo León, México
//  
//  RoBorregos Foxtrot, competing on:
//  LARC - Latin American and Brazilian Robotics Competition
//  VSSS - Very Small Size Soccer
//  
//  
//  INFORMATION
//  Information.cpp
//  
//  Last update: September 21, 2019
//               Ernesto Cervantes

#include "Information.h"

void Information::setRobotId(uint32_t &data, uint8_t id)
{
    id &= 0x07;
    uint32_t id32 = id;
    clearRobotId(data);
    data |= (id32 << 21);
}

void Information::clearRobotId(uint32_t &data)
{
    data &= 0xFF1FFFFF;
}

uint8_t Information::getRobotId(uint32_t data)
{
    return (data | 0x00E00000) >> 21;
}

void Information::setStop(uint32_t &data)
{
    data |= 0x00100000;
}

void Information::setSpeedLeft(uint32_t &data, bool forward, uint8_t speed)
{
    uint32_t speed32 = speed;
    clearSpeedLeft(data);
    data |= (speed32 << 8);
    if(forward)
        data |= 0x00020000;
}

void Information::setSpeedRight(uint32_t &data, bool forward, uint8_t speed)
{
    clearSpeedRight(data);
    data |= speed;
    if(forward)
        data |= 0x00010000;
}

void Information::clearStop(uint32_t &data)
{
    data &= 0xFFFEFFFF;
}

void Information::clearSpeedLeft(uint32_t &data)
{
    data &= 0xFFF7FFFF;
}

void Information::clearSpeedRight(uint32_t &data)
{
    data &= 0xFFFBFF00;
}

bool Information::hasStop(uint32_t data)
{
    return data & 0x00100000;
}

bool Information::hasSpeedLeft(uint32_t data)
{
    return data & 0x00080000;
}

bool Information::hasSpeedRight(uint32_t data)
{
    return data & 0x00040000;
}

bool Information::isLeftForward(uint32_t data)
{
    return data & 0x00020000;
}

bool Information::isRightForward(uint32_t data)
{
    return data & 0x00010000;
}

uint8_t Information::getSpeedLeft(uint32_t data)
{
    return (uint8_t) ((data & 0x0000FF00) >> 8);
}

uint8_t Information::getSpeedRight(uint32_t data)
{
    return (uint8_t) (data & 0x000000FF);
}