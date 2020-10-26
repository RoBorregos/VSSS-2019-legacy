//  RoBorregos
//  Tecnológico de Monterrey
//  Monterrey, Nuevo León, México
//  
//  RoBorregos Foxtrot, competing on:
//  LARC - Latin American and Brazilian Robotics Competition
//  VSSS - Very Small Size Soccer
//  
//  
//  SHAPE
//  Shape.h
//  
//  Last update: June 1, 2020
//               zjuarez

#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"
#include <chrono>
#include "Point.h"

class Shape{
    public:
        Shape(){
            currentPos = Point(0,0);
            finalPos = Point(0,0);
            dy = 0;
            dx = 0;
            ori = 0;
            finalOri = -1; 
            error=0;
            lastDistance = 0;
            speedAngle = 0;
        }
        
        float dx, dy, ori, finalOri,error, lastDistance, speedAngle;
        Point currentPos, finalPos;
        std::chrono::time_point<std::chrono::system_clock> lastTime;
        uint8_t id, role;
};

#endif
