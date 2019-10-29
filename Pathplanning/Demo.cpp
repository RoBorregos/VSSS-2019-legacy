#include "Pathplanning.h"

int main(){
    Point dep(0,0);
    Point obj(15,15);
    Pathplanning path(dep, obj, 2);
    path.obstacles[0].x = 7;
    path.obstacles[0].y = 7;
    path.obstacles[1].x = 5;
    path.obstacles[1].y = 9;
    path.obstacles[2].x = 19;
    path.obstacles[2].y = 0;
    path.obstacles[3].x = 19;
    path.obstacles[3].y = 0;
    path.obstacles[4].x = 19;
    path.obstacles[4].y = 0;
    path.shortPath(dep, obj);
    path.printData();
    return 0;
}