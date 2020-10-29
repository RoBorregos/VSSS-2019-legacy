#ifndef SIMULATORBRIDGE_H
#define SIMULATORBRIDGE_H

#include "../../../libraries/Shape/Shape.h"
#include "../../../libraries/Shape/Point.h"
#include "../../../libraries/Control/Control.h"
#include "Domain/Robot.h"
#include <math.h>
#include <vector>

class SimulatorBridge{

    public:

    std::vector<Shape> allies;
    Control myControl;
    
    void updateShapes(std::vector<vss::Robot> robots);
    void setFinalPos(int id, float x, float y);
    void setFinalAngle(int id, float finalAngle);
    void updateControl(){
        myControl = Control(allies);
    }
    SimulatorBridge(){
        std::vector<Shape> allies(3);
        this->allies = allies;
        myControl = Control(this->allies);
    }
};
#endif
