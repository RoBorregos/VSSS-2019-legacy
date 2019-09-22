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

#include "Figure.h"

Figure::Figure(float x, float y, float radius)
{
    this->x = x;
    this->y = y;
    this->radius = radius;
}

float Figure::getDistance(Figure a)
{
    return sqrt(pow(this->x - a.x, 2) + pow(this->y - a.y, 2));
}