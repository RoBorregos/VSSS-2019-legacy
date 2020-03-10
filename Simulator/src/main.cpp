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


using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSenderY;
IDebugSender *debugSenderY;
ICommandSender *commandSenderB;
IDebugSender *debugSenderB;

State state;

static const double pi = 3.141592;

double wrapMax(double x, double max)
{
    return fmod(max + fmod(x, max), max);
}
double wrapMinMax(double x, double min, double max)
{
    return min + wrapMax(x - min, max - min);
}
double distance(double x, double y, double x2, double y2)
{
    return 0.0113 * sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
}

struct param
{
    double ts = 0.01;
    double k[3] = {3.5, 4.5 * 0.7, 7.1 * 0.7}; //v,w,w  3.5,4.5,7.1
    double errorG[6] = {0, 0, 0, 0, 0, 0};
    std::chrono::time_point<std::chrono::system_clock> before[6];
    double r = 0.016;
    double l = 0.062;
    double s2 = 105;
    double s1 = 500;
    int min = 15;
    double distanceA[6] = {0, 0, 0, 0, 0, 0};
} rob;

void send_commands(std::vector<std::pair<double, double>>, std::vector<std::pair<double, double>>);
void send_debug();
void moveTo(std::string  team, int id, double x, double y, double angle, std::vector<std::pair<double,double>> &v);
std::pair<double,double> calculate(std::string team, int id, double x, double y, double angle);

double map(double val, double fromL, double fromH, double toL, double toH)
{
    return (val - fromL) * (toH - toL) / (fromH - fromL) + toL;
}

void moveTo(std::string team, int id, double x, double y, double angle, std::vector<std::pair<double, double>> &v)
{
    rob.min = id == 0 ? 2 : 15;
    std::pair<double, double> result = calculate(team, id, x, y, angle);
    v[id] = result;
}

std::pair<double, double> calculate(std::string team, int id, double x, double y, double angle)
{

    double REF_X, REF_Y, CUR_X, CUR_Y, REF_ANGLE, CUR_ANGLE, REF_SPEED, REF_ASPEED;
    if(team=="yellow"){
    CUR_X = state.teamYellow[id].x;
    CUR_Y = -state.teamYellow[id].y;
    CUR_ANGLE = ((int)(360 - state.teamYellow[id].angle) % 360) * pi / 180.0;
    }
    else{
    CUR_X = state.teamBlue[id].x;
    CUR_Y = -state.teamBlue[id].y;
    CUR_ANGLE = ((int)(360 - state.teamBlue[id].angle) % 360) * pi / 180.0;
    }

    CUR_ANGLE = wrapMinMax(CUR_ANGLE, -pi, pi);

    REF_X = x;
    REF_Y = -y;

    double difX = REF_X - CUR_X;
    double difY = REF_Y - CUR_Y;

    if (angle > -500)
    {
        REF_ANGLE = ((int)(360 - angle) % 360) * pi / 180.0;
    }
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

    double d = distance(REF_X, REF_Y, CUR_X, CUR_Y);
    REF_ANGLE = wrapMinMax(REF_ANGLE, -pi, pi);
    
    REF_SPEED = 5;
    REF_ASPEED = 1; 

    double ki = 0.87;
    double kd = 0.13;

    //REF_SPEED = Cur.speed<0? -5: 5;
    REF_X += rob.ts * REF_SPEED * cos(REF_ANGLE);
    REF_Y += rob.ts * REF_SPEED * sin(REF_ANGLE);

    REF_X = REF_X < 12 ? 12 : REF_X > 157 ? 157 : REF_X;
    REF_Y = REF_Y > -15 ? -15 : REF_Y < -110 ? -110 : REF_Y;

    difX = REF_X - CUR_X;
    difY = REF_Y - CUR_Y;

    double e1 = 0.0113 * difX * (cos(REF_ANGLE) + sin(REF_ANGLE));
    double e2 = 0.0113 * difY * (cos(REF_ANGLE) - sin(REF_ANGLE));
    double e3 = wrapMinMax(REF_ANGLE - CUR_ANGLE, -pi, pi);


    double k = 0.94;

    if ((d <= 0.24 || REF_X == 12 || REF_X == 157 || REF_Y == -110 || REF_Y == -15) && id != 0)
        k = 2.2;

    double v = REF_SPEED * cos(e3) + rob.k[0] * e1;
    double w = REF_ASPEED + k * rob.k[1] * REF_SPEED * e2 * +k * rob.k[2] * REF_SPEED * sin(e3);

    double right = (v * 2.0 + w * rob.l) / (2.0 * rob.r);
    double left = (v * 2.0 - w * rob.l) / (2.0 * rob.r);

    int newId = team == "yellow" ? id : 3 + id;

    auto now = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(now - rob.before[newId]).count();

    double prop = 1;
    double errorD;

    if (time > 180)
    {
        rob.errorG[newId] = rob.errorG[newId] * 0.8 + (d)*time * 0.001;
        errorD = (d - rob.distanceA[newId]) / (time * 0.001);

        rob.distanceA[newId] = d;
        rob.before[newId] = now;

        prop += errorD * kd + rob.errorG[newId] * ki;
  //      std::cout << "prop: " << prop << std::endl;
        right *= prop;
        left *= prop;
    }

    right = map(right, -rob.s1, rob.s1, -rob.s2, rob.s2);
    left = map(left, -rob.s1, rob.s1, -rob.s2, rob.s2);

    if (right > -rob.min && right < rob.min)
        right = right < 0 ? -rob.min : rob.min;

    if (left > -rob.min && left < rob.min)
        left = left < 0 ? -rob.min : rob.min;

 //   std::cout<<team << " "<<id<<" right: "<<right<<" left: "<<left<<std::endl;
    return std::make_pair(right, left);
}

std::vector<std::pair<double, double>> velocitiesYellow;
std::vector<std::pair<double, double>> velocitiesBlue;

int main(int argc, char **argv)
{
    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSenderY = new CommandSender();
    debugSenderY = new DebugSender();

    commandSenderB = new CommandSender();
    debugSenderB = new DebugSender();

    stateReceiver->createSocket();
    commandSenderY->createSocket(TeamType::Yellow);
    debugSenderY->createSocket(TeamType::Yellow);

    commandSenderB->createSocket(TeamType::Blue);
    debugSenderB->createSocket(TeamType::Blue);

    while (true)
    {
        state = stateReceiver->receiveState(FieldTransformationType::None);

        velocitiesYellow = {std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0)};
        velocitiesBlue = {std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0)};
      
       // sTeam, id, x, y, angle (if not -1000), vectorVelocities.

        moveTo("yellow", 0, 158, state.ball.y < 46 ? 46 : state.ball.y > 86 ? 86 : state.ball.y, -1000, velocitiesYellow);
        moveTo("yellow", 1, state.ball.x, state.ball.y, -1000, velocitiesYellow);
        moveTo("yellow", 2, state.ball.x, state.ball.y, -1000, velocitiesYellow);

        moveTo("blue", 0, 10, state.ball.y < 46 ? 46 : state.ball.y > 86 ? 86 : state.ball.y, -1000, velocitiesBlue);
        moveTo("blue", 1, state.ball.x, state.ball.y, -1000, velocitiesBlue);
        moveTo("blue", 2, state.ball.x, state.ball.y, -1000, velocitiesBlue);

        send_commands(velocitiesYellow, velocitiesBlue);
    }

    return 0;
}

void send_commands(std::vector<std::pair<double, double>> velY, std::vector<std::pair<double, double>> velB)
{
    Command commandY;
    Command commandB;

    for (int i = 0; i < 3; i++)
    commandY.commands.push_back(WheelsCommand(velY[i].first, velY[i].second));
       
    commandSenderY->sendCommand(commandY);

    for (int i = 0; i < 3; i++)
    commandB.commands.push_back(WheelsCommand(velB[i].first, velB[i].second));
    
    commandSenderB->sendCommand(commandB);


}
void send_debug()
{
    vss::Debug debug;

    for (unsigned int i = 0; i < 3; i++)
    {
        debug.stepPoints.push_back(Point(85 + rand() % 20, 65 + rand() % 20));
    }

    for (unsigned int i = 0; i < 3; i++)
    {
        debug.finalPoses.push_back(Pose(85 + rand() % 20, 65 + rand() % 20, rand() % 20));
    }

    for (unsigned int i = 0; i < 3; i++)
    {
        vss::Path path;
        path.points.push_back(Point(85, 65));
        path.points.push_back(Point(85 + rand() % 20, 65 + rand() % 20));
    }

    debugSenderY->sendDebug(debug);
}
