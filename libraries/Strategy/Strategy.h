#ifndef Strategy_H
#define Strategy_H

#include "stdint.h"
#include "../Shape/Point.h"
#include "../Shape/Shape.h"
#include "../Shape/Ball.h"
#include "../Shape/Robot.h"

class Strategy{
    private:
        Robot allies[3];
        Robot enemies[3];
    public: 
        
        Strategy(Robot &a0,Robot &a1,Robot &a2, Robot &e0, Robot &e1,Robot &e2){
            this->allies[0] = a0;
            this->allies[1] = a1;
            this->allies[2] = a2;
            this->enemies[0] = e0;
            this->enemies[1] = e1;
            this->enemies[2] = e2;
        }
        
};

#endif