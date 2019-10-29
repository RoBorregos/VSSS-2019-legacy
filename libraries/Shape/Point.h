#include <math.h>
#include <iostream>

class Point{        
        
    public:
        Point();
        Point(float x, float y);
        float distance(Point reference);
        void printData();
        float x, y;

};

Point::Point(){}

Point::Point(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Point::distance(Point ref){
    return sqrt(pow(x-ref.x, 2) + pow(y-ref.y, 2));
}

void Point::printData(){
    std::cout << "(" << x << "," << y << ")" << std::endl;
}