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

void Control::move(int id, double &rightV, double &leftV)
{
    double REF_X, REF_Y, CUR_X, CUR_Y, REF_ANGLE, CUR_ANGLE, REF_SPEED, REF_ASPEED;

    CUR_X = allies[id].currentPos.x;
    CUR_Y = allies[id].currentPos.y;
    CUR_ANGLE = ((int)(360 - allies[id].ori) % 360) * pi / 180.0;
 
    CUR_ANGLE = wrapMinMax(CUR_ANGLE, -pi, pi);

    REF_X = allies[id].finalPos.x;
    REF_Y = allies[id].finalPos.y * -1;
    REF_ANGLE = allies[id].finalOri; 

    double difX = REF_X - CUR_X;
    double difY = REF_Y - CUR_Y;

    // -1 is a generic final angle (use this to reach  the final position in a straight line)
    if (REF_ANGLE !=-1)
        REF_ANGLE = ((int)(360 - REF_ANGLE) % 360) * pi / 180.0;
    else
    {
        if (difX == 0 && difY == 0)
            REF_ANGLE = 0;
        else if (difX == 0)
            REF_ANGLE = pi / 2.0;
        else
            REF_ANGLE = atan(difY / difX);
        if (CUR_X > REF_X)
            REF_ANGLE += pi;
    }

    REF_ANGLE = wrapMinMax(REF_ANGLE, -pi, pi);

    REF_SPEED = 3; //NEEDS TESTING (GENERIC)
    REF_ASPEED = 1; //NEEDS TESTING (GENERIC)


    REF_X += ts * REF_SPEED * cos(REF_ANGLE);
    REF_Y += ts * REF_SPEED * sin(REF_ANGLE);

    REF_X = REF_X < 12 ? 12 : REF_X > 157 ? 157 : REF_X; //LIMITS
    REF_Y = REF_Y > -15 ? -15 : REF_Y < -110 ? -110 : REF_Y;  //LIMITS

    difX = REF_X - CUR_X;
    difY = REF_Y - CUR_Y;

    double e1 = 0.0113 * difX * (cos(REF_ANGLE) + sin(REF_ANGLE));
    double e2 = 0.0113 * difY * (cos(REF_ANGLE) - sin(REF_ANGLE));
    double e3 = wrapMinMax(REF_ANGLE - CUR_ANGLE, -pi, pi);

    Point curPoint = Point(CUR_X,CUR_Y);
    Point refPoint = Point(REF_X,REF_Y);

    double d = curPoint.distance(refPoint);

    double multiply = 1;

        //angular speed should be greater if its close to a wall or the objective to unstuck (not the goalie)
    if ((d <= 0.22 || REF_X <= 12 || REF_X >= 157 || REF_Y <= -110 || REF_Y >= -15) && id != 0)
        multiply = 2;

        //calculations of linear and angular speed
    double v = REF_SPEED * cos(e3) + k[0] * e1;
    double w = REF_ASPEED + multiply * k[1] * REF_SPEED * e2 * + multiply * k[2] * REF_SPEED * sin(e3);

        //calculations of right and left velocities
    double right = (v * 2.0 + w * l) / (2.0 * r);
    double left = (v * 2.0 - w * l) / (2.0 * r);

        //some Integrative-Derivative considerations on right and left
        //alternative is to do this before. for the w and v

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

    right = map(right, -s1, s1, -s2, s2);
    left = map(left, -s1, s1, -s2, s2);

    if (right > -min && right < min)
        right = right < 0 ? -min : min;

    if (left > -min && left < min)
        left = left < 0 ? -min : min;

rightV = right;
leftV = left;
}

