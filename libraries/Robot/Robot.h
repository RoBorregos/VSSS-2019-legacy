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
//               Osvaldo Alvarez

#ifndef ROBOT_H
#define ROBOT_H

#include "../Figure/Figure.h"
#include "stdint.h"

class Robot : public Figure {
    public:
        Robot(): Figure(){
            //constructor
        }

        float xFin, yFin, ori;
        uint8_t id, role;

        void test(Figure x) {
            x.
        }
};

#endif
