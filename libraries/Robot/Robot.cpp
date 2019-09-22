//  RoBorregos
//  Tecnológico de Monterrey
//  Monterrey, Nuevo León, México
//  
//  RoBorregos Foxtrot, competing on:
//  LARC - Latin American and Brazilian Robotics Competition
//  VSSS - Very Small Size Soccer
//  
//  
//  ROBOT
//  Robot.h
//  
//  Last update: September 22, 2019
//               Ernesto Cervantes

#include "Robot.h"

Robot::Robot(float x, float y, float radius, uint8_t id, uint8_t role, bool isOurs): Figure(x, y, radius)
{
    this->id = id;
    this->role = role;
    this->isOurs = isOurs;
}