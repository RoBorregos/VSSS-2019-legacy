//  RoBorregos
//  Tecnológico de Monterrey
//  Monterrey, Nuevo León, México
//  
//  RoBorregos Foxtrot, competing on:
//  LARC - Latin American and Brazilian Robotics Competition
//  VSSS - Very Small Size Soccer
//  
//  
//  FIGURE
//  Figure.h
//  
//  Last update: September 22, 2019
//               Osvaldo Alvarez

#ifndef FIGURE_H
#define FIGURE_H

#include <chrono>

class Figure {
    public:
        Figure(){
            dy = 0;
            dx = 0;
        }

        float xAct, yAct, dy, dx;
        std::chrono::time_point<std::chrono::system_clock> lastTime;
};

#endif
