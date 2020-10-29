#include "SimulatorBridge.h"

void SimulatorBridge::updateShapes(std::vector<vss::Robot> robots){
    for(int i=0; i<3; i++){
        allies[i].currentPos = Point(robots[i].x,robots[i].y);
        allies[i].ori = robots[i].angle;
        allies[i].dx = robots[i].speedX;
        allies[i].dy = robots[i].speedY;
        allies[i].speedAngle = robots[i].speedAngle;
        //debuging
        //std::cout<<"id: "<<i<<" current pos: "<<allies[i].currentPos.x<<" and "<<allies[i].currentPos.y<<std::endl;
    }
}
void SimulatorBridge::setFinalPos(int id, float x, float y){
    allies[id].finalPos = Point(x,y);

}
void SimulatorBridge::setFinalAngle(int id, float referenceAngle){
    allies[id].finalOri = referenceAngle;
}
