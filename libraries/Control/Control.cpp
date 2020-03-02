#include "Control.h"

double Control::wrapMinMax(double x, double min, double max){
    return min+wrapMax(x-min,max-min);
}
double Control::wrapMax(double x, double max){
    return fmod(max+fmod(x,max),max);

}

double Control::getSpeed(double x, double y){
    return 0.0113*sqrt(x*x+y*y);
}

double Control::map(double val,double fromL, double fromH, double toL, double toH){
    return (val-fromL)*(toH-toL)/(fromH-fromL)+toL;
}

void  Control::move(int id, double &right, double &left){

team[id].finalPos.y *=-1;
team[id].currentPos.y *=-1;

double difX = team[id].finalPos.x - team[id].currentPos.x;
double difY = team[id].finalPos.y - team[id].currentPos.y;

double goAngle,goSpeed=5,goASpeed=1,currAngle;

currAngle = (int)(360-team[id].ori)%360*pi/180.0;
currAngle = wrapMinMax(currAngle,-pi,pi);

if(difX==0 && difY==0)
goAngle = 0;
else if(difX==0)
goAngle = pi/2.0;
else 
goAngle = atan(difY/difX);

if(team[id].currentPos.x>team[id].finalPos.x)
goAngle += pi;

goAngle=wrapMinMax(goAngle,-pi,pi);

//speed = Cur.speed<0? -5: 5;
team[id].finalPos.x += ts*goSpeed*cos(goAngle);
team[id].finalPos.y += ts*goSpeed*sin(goAngle);

difX = team[id].finalPos.x - team[id].currentPos.x;
difY = team[id].finalPos.y - team[id].currentPos.y;

double e1 = 0.0113*difX*(cos(goAngle)+sin(goAngle));
double e2 = 0.0113*difY*(cos(goAngle)-sin(goAngle));
double e3 = wrapMinMax(goAngle-currAngle,-pi,pi);

double v = goSpeed*cos(e3)+k[0]*e1;
double w = goASpeed+k[1]*goSpeed*e2*+k[2]*goSpeed*sin(e3);

 right = (v*2.0+w*l)/(2.0*r);
 left = (v*2.0-w*l)/(2.0*r);
        
        right = map(right,-s1,s1,-s2,s2);
        left = map(left,-s1,s1,-s2,s2);


        if(right>-min && right<min)
        right = right<0? -min: min;

        if(left>-min && left<min)
        left = left<0? -min: min;

} 