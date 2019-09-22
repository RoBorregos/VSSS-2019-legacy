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
//               Ernesto Cervantes

#ifndef FIGURE_H
#define FIGURE_H

#include <cmath>

class Figure
{
public:
    Figure(float x, float y, float radius);
    float getDistance(Figure a);

private:
    float x, y, radius;
};

#endif