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
//  Last update: September 22, 2019
//               Osvaldo Alvarez

#ifndef SHAPE_H
#define SHAPE_H

#include "stdint.h"
#include "Point.h"

class Shape{
    private:
        int id;
        double vx;
        double vy;
        Point coords;
        Point idealCoords;
 
    public:
        Shape(){

        }

        void changeId(int id ){
            this->id = id;
        }

        void changeCurrentCoords(Point coords){
            this->coords.changeCoords(coords);
        }

        void changeIdealCoords(Point idealCoords){
            this->idealCoords.changeCoords(idealCoords);
        }

        void changeVel(double vx, double vy){
            this->vx = vx;
            this->vy = vy;            
        }

        void print(){
            std::cout<<"ID: "<<this->id<<"\n"<<"Current Coords: "<<this->coords.print()<<"Ideal Coords: "<<this->idealCoords.print()<<"State: "<<"\n";
        }

};

#endif
