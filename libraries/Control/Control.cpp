#include "Control.h"

double Control::wrapMinMax(double x, double min, double max){
    return min+wrapMax(x-min,max-min);
}
double Control::wrapMax(double x, double max){
    return fmod(max+fmod(x,max),max);

}

double Control::getSpeed(double dx, double dy){
    return 0.0113*sqrt(dx*dx+dy*dy);
}

double Control::map(double val,double fromL, double fromH, double toL, double toH){
    return (val-fromL)*(toH-toL)/(fromH-fromL)+toL;
}

float Control::generateGenericFinalAngle(int id){

    float REF_ANGLE;
    float difX = allies[id].finalPos.x - allies[id].currentPos.x;
    float difY = allies[id].currentPos.y - allies[id].finalPos.y;

    if (difX == 0 && difY == 0)
            REF_ANGLE = 0;
        else if (difX == 0)
            REF_ANGLE = pi / 2.0;
        else{
            REF_ANGLE = atan(difY / difX);
        if (allies[id].currentPos.x > allies[id].finalPos.x)
            REF_ANGLE += pi;
        }
    
    REF_ANGLE = wrapMinMax(REF_ANGLE, -pi, pi);
    return REF_ANGLE;
}

void Control::move(Shape &robot, double &rightV, double &leftV)
{
    double REF_X, REF_Y, CUR_X, CUR_Y, REF_ANGLE, CUR_ANGLE, REF_SPEED, REF_ASPEED;

    CUR_X = robot.currentPos.x;
    CUR_Y = robot.currentPos.y*-1;
    CUR_ANGLE = ((int)(360 - robot.ori) % 360) * pi / 180.0;
    CUR_ANGLE = wrapMinMax(CUR_ANGLE, -pi, pi);

    REF_X = robot.finalPos.x;
    REF_Y = robot.finalPos.y * -1;
    REF_ANGLE = robot.finalOri; 

    //debug
    std::cout<<"CONTROL CUR X AND REF X: "<< CUR_X << " "<< REF_X<<std::endl;
    std::cout<<"CONTROL CUR Y AND REF Y: "<< CUR_Y << " "<< REF_Y<<std::endl;
    std::cout<<"CONTROL CUR A AND REF A: "<< CUR_ANGLE << " "<< REF_ANGLE<<std::endl;

    double difX = REF_X - CUR_X;
    double difY = REF_Y - CUR_Y;

    REF_SPEED = 1.5; //NEEDS TESTING (GENERIC)
    REF_ASPEED = 2; //NEEDS TESTING (GENERIC)

    REF_X += ts * REF_SPEED * cos(REF_ANGLE);
    REF_Y += ts * REF_SPEED * sin(REF_ANGLE);

    REF_X = REF_X < 12 ? 12 : REF_X > 157 ? 157 : REF_X; //LIMITS
    REF_Y = REF_Y > -15 ? -15 : REF_Y < -110 ? -110 : REF_Y;  //LIMITS

    difX = REF_X - CUR_X;
    difY = REF_Y - CUR_Y;

    double e1 = kSimulator * difX * (cos(REF_ANGLE) + sin(REF_ANGLE));
    double e2 = kSimulator * difY * (cos(REF_ANGLE) - sin(REF_ANGLE));
    double e3 = wrapMinMax(REF_ANGLE - CUR_ANGLE, -pi, pi);

        //calculations of linear and angular speed
    double v = REF_SPEED * cos(e3) + k[0] * e1;
    double w = REF_ASPEED + ( k[1] * REF_SPEED * e2 * +   k[2] * REF_SPEED * sin(e3));

        //calculations of right and left velocities
    double right = (v * 2.0 + w * l) / (2.0 * r);
    double left = (v * 2.0 - w * l) / (2.0 * r);

        //some Integrative-Derivative considerations on right and left
        //alternative is to do this before. for the w and v
/*
    auto now = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(now - allies[id].lastTime).count();
    double prop = 1;
    double errorD;

    if (time > 180)
    {
        allies[id].error = allies[id].error * 0.8 + (d)*time * 0.001;
        errorD = (d - allies[id].lastDistance) / (time * 0.001);

        allies[id].lastDistance = d;
        allies[id].lastTime = now;

        prop += errorD * kd + allies[id].error * ki;

        std::cout << "prop: " << prop << std::endl;  //debugging skills

        right *= prop;
        left *= prop;
    }
    */
/*
    right = map(right, -s1, s1, -s2, s2);
    left = map(left, -s1, s1, -s2, s2);

    if (right > -min && right < min)
        right = right < 0 ? -min : min;

    if (left > -min && left < min)
        left = left < 0 ? -min : min;
*/
std::cout<<right<<" "<<left<<std::endl;
rightV = right;
leftV = left;
}

std::vector<std::pair<double,double>> Control::moveAll(){

    std::vector<std::pair<double,double>> result;
    double r,l;
    
    for(int i=0; i<3; i++){
    r=0,l=0;
    move(allies[i],r,l);
    result.push_back(std::make_pair(r,l));
    }

    return result;
}

