
#ifndef BALL_H
#define BALL_H

#include "stdint.h"
#include "Point.h"
#include "Shape.h"

class Ball : private Shape {

    private:

    public: 
        Ball(){

        }
        Ball(int id, double vx, double vy, Point coords, Point idealCoords){
            this->changeCurrentCoords(coords);
            this->changeIdealCoords(idealCoords);
            this->changeVel(vx,vy);
            this->changeId(id);
        }


};


#endif