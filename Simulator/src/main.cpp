/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <Communications/DebugSender.h>
#include "cstdlib"
#include <math.h>
#include <chrono>

static const double pi = 3.141592;
using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSender;
IDebugSender *debugSender;

State state;

double wrapMax(double x, double max)
{return fmod(max+fmod(x,max),max);}
double wrapMinMax(double x, double min, double max)
{return min+wrapMax(x-min,max-min);}
double distance(double x, double y,double x2, double y2)
{return 0.0113*sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));}

double s2 =105;
double s1 = 500;
int min = 15;

struct position{
double x;
double y;
double angle;
double speed;
double Aspeed;

position(double x_,
double y_){
x = x_;
y=-1.0*y_;
angle = 0;
speed = 5;
Aspeed = 1;
}
position(int id){
x = state.teamYellow[id].x;
y=-1.0*state.teamYellow[id].y;
angle = ((int)(360-state.teamYellow[id].angle)%360)*pi/180.0;
angle = wrapMinMax(angle,-pi,pi);
speed = 0.0113*sqrt(state.teamYellow[id].speedX*state.teamYellow[id].speedX+state.teamYellow[id].speedY*state.teamYellow[id].speedY);
Aspeed = 1;
}
void print(){
std::cout<<"Pos X: "<<x<<std::endl;
std::cout<<"Pos Y: "<<y<<std::endl;
std::cout<<"Angle: "<<angle<<std::endl;
std::cout<<"Speed: "<<speed<<std::endl;
std::cout<<"Angular Speed: "<<Aspeed<<std::endl;
}
position(char p)
{
x = state.ball.x;
y=-1.0*state.ball.y;
angle = 0;
speed = 5;
Aspeed = 0;
}
};

struct param{
double ts=0.01;
double k[3] = {3.5,4.5*0.7,7.1*0.7}; //v,w,w  3.5,4.5,7.1
double errorG[3]={0,0,0};
std::chrono::time_point<std::chrono::system_clock> before[3];
double r=0.016;
double l = 0.062;
double distanceA[3]={0,0,0};
}rob;

void send_commands(std::vector<std::pair<double,double>>);
void send_debug();
std::pair<double,double> calculate(int id,position Cur, position Ref);
double map(double val,double fromL, double fromH, double toL, double toH)
{return (val-fromL)*(toH-toL)/(fromH-fromL)+toL;}


void moveTo(int id, double x, double y, std::vector<std::pair<double,double>> &velocities)
{
position current (id);
position reference (x,y);
min = id==0? 2: 15;
std::pair<double,double> result = calculate(id,current,reference);
velocities[id] = result;
//std::cout<<id<<": "<<result.first<<" "<<result.second<<std::endl;

}

std::vector <std::pair<double,double>> velocities;

int main(int argc, char** argv){
    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSender = new CommandSender();
    debugSender = new DebugSender();

    stateReceiver->createSocket();
    commandSender->createSocket(TeamType::Yellow);
    debugSender->createSocket(TeamType::Yellow);
    //position ref (85,65,180,5,0);

    while(true){
        state = stateReceiver->receiveState(FieldTransformationType::None);
        velocities = {std::make_pair(0,0),std::make_pair(0,0),std::make_pair(0,0)};
        // moveTo's (robot id, x, y, velocities);
       
        moveTo(0, 158,state.ball.y<46? 46:state.ball.y>86? 86: state.ball.y,velocities);
        //moveTo(1, state.ball.x,state.ball.y,velocities);
        moveTo(2, state.ball.x,state.ball.y,velocities);

       send_commands(velocities);
        
    }

    return 0;
}

void send_commands(std::vector<std::pair<double,double>> vel){
    Command command;
   
   for(int i = 0; i<3; i++)
command.commands.push_back(WheelsCommand(vel[i].first, vel[i].second));
    
    commandSender->sendCommand(command);
}

std::pair<double,double> calculate(int id, position Cur, position Ref){

double ki = 0.87;
double kd = 0.13;

double difX = Ref.x-Cur.x;
double difY = Ref.y-Cur.y;

if(difX==0 && difY==0)
Ref.angle = 0;
else if(difX==0)
Ref.angle = pi/2.0;
else 
Ref.angle = atan(difY/difX);

if(Cur.x>Ref.x)
Ref.angle += pi;

Ref.angle=wrapMinMax(Ref.angle,-pi,pi);

//Ref.speed = Cur.speed<0? -5: 5;
Ref.x += rob.ts*Ref.speed*cos(Ref.angle);
Ref.y += rob.ts*Ref.speed*sin(Ref.angle);


Ref.x = Ref.x<12 ? 12: Ref.x>157? 157 : Ref.x;
Ref.y = Ref.y>-15 ? -15 : Ref.y<-110? -110 : Ref.y;

difX = Ref.x - Cur.x;
difY = Ref.y - Cur.y;

double e1 = 0.0113*difX*(cos(Ref.angle)+sin(Ref.angle));
double e2 = 0.0113*difY*(cos(Ref.angle)-sin(Ref.angle));
double e3 = wrapMinMax(Ref.angle-Cur.angle,-pi,pi);

double d = distance(Ref.x,Ref.y,Cur.x,Cur.y);

double k = 1;
if((d<=0.22 || Ref.x==12 || Ref.x==157 || Ref.y == -110 || Ref.y==-15) && id!=0)
k = 2;

double v = Ref.speed*cos(e3)+rob.k[0]*e1;
double w = Ref.Aspeed+k*rob.k[1]*Ref.speed*e2*+k*rob.k[2]*Ref.speed*sin(e3);

Cur.speed = v;
Cur.Aspeed = w;

double right = (Cur.speed*2.0+Cur.Aspeed*rob.l)/(2.0*rob.r);
double left = (Cur.speed*2.0-Cur.Aspeed*rob.l)/(2.0*rob.r);       
    
    auto now = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(now-rob.before[id]).count();
    double prop = 1;
    double errorD;

    
    if(time>180){
    rob.errorG[id] = rob.errorG[id]*0.8 + (d)*time*0.001;
    errorD = (d-rob.distanceA[id])/(time*0.001);
    
    rob.distanceA[id] = d;
    rob.before[id] = now;

    prop +=  errorD*kd + rob.errorG[id]*ki;
    std::cout<<"prop: "<<prop<<std::endl;
    right*=prop;
    left*=prop;
    }

        right = map(right,-s1,s1,-s2,s2);
        left = map(left,-s1,s1,-s2,s2);


        if(right>-min && right<min)
        right = right<0? -min: min;

        if(left>-min && left<min)
        left = left<0? -min: min;

    return std::make_pair(right,left);
}

void send_debug(){
    vss::Debug debug;

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.stepPoints.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.finalPoses.push_back(Pose(85 + rand()%20, 65 + rand()%20, rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Path path;
        path.points.push_back(Point(85, 65));
        path.points.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    debugSender->sendDebug(debug);
}