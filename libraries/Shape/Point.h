#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <iostream>

class Point{        
    public:
        Point(){};

        Point(float x, float y){
            this->x = x;
            this->y = y;
        }

        float distance(Point ref){
            return sqrt(pow(x-ref.x, 2) + pow(y-ref.y, 2));
        }
        void printData(){
             std::cout << "(" << x << "," << y << ")" << std::endl;
        }
        float x, y;

};


#endif 