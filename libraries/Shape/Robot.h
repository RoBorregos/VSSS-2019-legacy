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


#ifndef ROBOT_H
#define ROBOT_H

#include "stdint.h"
#include "Point.h"
#include "Shape.h"

enum Task {
    defend,
    middle,
    attack
};

class Robot : private Shape {

    private: 
        Task role;
        double theta;

    public:
        Robot(){

        }
        Robot(int id, double vx, double vy, Point coords, Point idealCoords,Task role, double theta){
            this->changeCurrentCoords(coords);
            this->changeIdealCoords(idealCoords);
            this->changeVel(vx,vy);
            this->changeId(id);
            this->role = role;
            this->theta = theta;
        }


};

#endif
