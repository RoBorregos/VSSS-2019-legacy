//  RoBorregos
//  Tecnológico de Monterrey
//  Monterrey, Nuevo León, México
//  
//  RoBorregos Foxtrot, competing on:
//  LARC - Latin American and Brazilian Robotics Competition
//  VSSS - Very Small Size Soccer
//  
//  zjuarez

#ifndef CONTROL_H
#define CONTROL_H

#include "stdint.h"
#include "../Shape/Shape.h"
#include "../Shape/Point.h"
#include <math.h>
#include <vector>
class Control{

    private:

        std::vector<Shape> allies;

        double wrapMinMax(double x, double min, double max); //keep angle wraped in range(pi, -pi)
        double wrapMax(double x, double max);
        double getSpeed(double dx, double dy); //get |tangent speed with dx and dy
        double map(double val,double fromL, double fromH, double toL, double toH);

        double pi = 3.141592;
        double s2 =100; //map values
        double s1 = 500;
        double ts=0.01; //constants
        double k[3] = {3.5,4.5,7.1};
        double r=0.016; //radius
        double l = 0.062; //length between wheels
        int min = 15; //minium vel (+,-)
        double ki = 0.87;
        double kd = 0.13;

    public:
        Control(std::vector<Shape> &allies){
            this->allies = allies;
        }
        void move(int id, double &right, double &left);                 
};

#endif
