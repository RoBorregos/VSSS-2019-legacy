#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <iostream>

class Point{   
         
    private: 
        int x;
        int y;

    public:
        Point(){};

        void changeCoords(Point p){
            this->x = p.x;
            this->y = p.y;
        }

        double getdistance(Point p){
            return sqrt((p.x-this->x)*(p.x-this->x) + (p.y-this->y)*(p.y-this->y));
        }
        
        void print(){
            std::cout << "(" << x << "," << y << ")" << "\n";
        }
};


#endif 