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

#ifndef ROBOT_H
#define ROBOT_H

#include "Figure.h"
#include "stdint.h"

enum role_types{goalkeeper, defense, attacker};

class Robot : public Figure
{
public:
    Robot(float x, float y, float radius, uint8_t id, uint8_t role, bool isOurs);

private:
    uint8_t id, role;
    bool isOurs;
};

#endif