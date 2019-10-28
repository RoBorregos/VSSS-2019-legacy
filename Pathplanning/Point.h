#include <math.h>
#include <iostream>

using namespace std;

class Point{        
        
    public:
        Point();
        Point(int x, int y);
        float distance(Point reference);
        void printData();
        float x;
        float y;

};

Point::Point(){}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

float Point::distance(Point ref){
    return sqrt(pow(x-ref.x, 2) + pow(y-ref.y, 2));
}

void Point::printData(){
    cout << "(" << x << "," << y << ")" << endl;
}