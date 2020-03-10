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
ICommandSender *commandSenderY;
IDebugSender *debugSenderY;
ICommandSender *commandSenderB;
IDebugSender *debugSenderB;

State state;

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

double s2 = 105;
double s1 = 500;
int min = 15;

struct param
{
    double ts = 0.01;
    double k[3] = {3.5, 4.5 * 0.7, 7.1 * 0.7}; //v,w,w  3.5,4.5,7.1
    double errorG[6] = {0, 0, 0, 0, 0, 0};
    std::chrono::time_point<std::chrono::system_clock> before[6];
    double r = 0.016;
    double l = 0.062;
    double distanceA[6] = {0, 0, 0, 0, 0, 0};
} rob;

void send_commands(std::vector<std::pair<double, double>>, std::vector<std::pair<double, double>>);
void send_debug();
void moveTo(std::string  team, int id, double x, double y, double angle, std::vector<std::pair<double,double>> &v))
std::pair<double,double> calculate(std::string team, int id, double x, double y, double angle);

double map(double val, double fromL, double fromH, double toL, double toH)
{
    return (val - fromL) * (toH - toL) / (fromH - fromL) + toL;
}

void moveTo(std::string team, int id, double x, double y, double angle, std::vector<std::pair<double, double>> &v)
{
    min = id == 0 ? 2 : 15;
    std::pair<double, double> result = calculate(team, id, x, y, angle);
    v[id] = result;
}

std::pair<double, double> calculate(std::string team, int id, double x, double y, double angle)
{

    double REF_X, REF_Y, CUR_X, CUR_Y, REF_ANGLE, CUR_ANGLE, REF_SPEED, REF_ASPEED;
    std::string t = "team" + team;

    CUR_X = state.t[id].x;
    CUR_Y = state.t[id].y;
    CUR_ANGLE = ((int)(360 - state.t[id].angle) % 360) * pi / 180.0;
    CUR_ANGLE = wrapMinMax(CUR_ANGLE, -pi, pi);

    REF_X = x;
    REF_Y = y * -1;

    double difX = REF_X - CUR_X;
    double difY = REF_Y - CUR_Y;

    if (REF_ANGLE > -10)
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

    REF_ANGLE = wrapMinMax(REF_ANGLE, -pi, pi);

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

    double d = distance(REF_X, REF_Y, CUR_X, CUR_Y);

    double k = 1;

    if ((d <= 0.22 || REF_X == 12 || REF_X == 157 || REF_Y == -110 || REF_Y == -15) && id != 0)
        k = 2;

    double v = REF_SPEED * cos(e3) + rob.k[0] * e1;
    double w = REF_ASPEED + k * rob.k[1] * REF_SPEED * e2 * +k * rob.k[2] * REF_SPEED * sin(e3);

    double right = (v * 2.0 + w * rob.l) / (2.0 * rob.r);
    double left = (v * 2.0 - w * rob.l) / (2.0 * rob.r);

    int newId = team == "Yellow" ? id : 3 + id;

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
        std::cout << "prop: " << prop << std::endl;
        right *= prop;
        left *= prop;
    }

    right = map(right, -s1, s1, -s2, s2);
    left = map(left, -s1, s1, -s2, s2);

    if (right > -min && right < min)
        right = right < 0 ? -min : min;

    if (left > -min && left < min)
        left = left < 0 ? -min : min;

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

        moveTo("Yellow", 0, 158, state.ball.y < 46 ? 46 : state.ball.y > 86 ? 86 : state.ball.y, -100, velocitiesYellow);
        moveTo("Yellow", 0, state.ball.x, state.ball.y, -100, velocitiesYellow);
        moveTo("Yellow", 0, state.ball.x, state.ball.y, -100, velocitiesYellow);

        moveTo("Blue", 0, 10, state.ball.y < 46 ? 46 : state.ball.y > 86 ? 86 : state.ball.y, -100, velocitiesBlue);
        moveTo("Blue", 0, state.ball.x, state.ball.y, -100, velocitiesBlue);
        moveTo("Blue", 0, state.ball.x, state.ball.y, -100, velocitiesBlue);

        send_commands(velocitiesYellow, velocitiesBlue);
    }

    return 0;
}

void send_commands(std::vector<std::pair<double, double>> velY, std::vector<std::pair<double, double>> velB, )
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

std::pair<double, double> calculate(int id, position Cur, position Ref)
{

    double ki = 0.87;
    double kd = 0.13;

    double difX = Ref.x - Cur.x;
    double difY = Ref.y - Cur.y;

    if (difX == 0 && difY == 0)
        Ref.angle = 0;
    else if (difX == 0)
        Ref.angle = pi / 2.0;
    else
        Ref.angle = atan(difY / difX);

    if (Cur.x > Ref.x)
        Ref.angle += pi;

    Ref.angle = wrapMinMax(Ref.angle, -pi, pi);

    //Ref.speed = Cur.speed<0? -5: 5;
    Ref.x += rob.ts * Ref.speed * cos(Ref.angle);
    Ref.y += rob.ts * Ref.speed * sin(Ref.angle);

    Ref.x = Ref.x < 12 ? 12 : Ref.x > 157 ? 157 : Ref.x;
    Ref.y = Ref.y > -15 ? -15 : Ref.y < -110 ? -110 : Ref.y;

    difX = Ref.x - Cur.x;
    difY = Ref.y - Cur.y;

    double e1 = 0.0113 * difX * (cos(Ref.angle) + sin(Ref.angle));
    double e2 = 0.0113 * difY * (cos(Ref.angle) - sin(Ref.angle));
    double e3 = wrapMinMax(Ref.angle - Cur.angle, -pi, pi);

    double d = distance(Ref.x, Ref.y, Cur.x, Cur.y);

    double k = 1;
    if ((d <= 0.22 || Ref.x == 12 || Ref.x == 157 || Ref.y == -110 || Ref.y == -15) && id != 0)
        k = 2;

    double v = Ref.speed * cos(e3) + rob.k[0] * e1;
    double w = Ref.Aspeed + k * rob.k[1] * Ref.speed * e2 * +k * rob.k[2] * Ref.speed * sin(e3);

    Cur.speed = v;
    Cur.Aspeed = w;

    double right = (Cur.speed * 2.0 + Cur.Aspeed * rob.l) / (2.0 * rob.r);
    double left = (Cur.speed * 2.0 - Cur.Aspeed * rob.l) / (2.0 * rob.r);

    auto now = std::chrono::system_clock::now();
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(now - rob.before[id]).count();
    double prop = 1;
    double errorD;

    if (time > 180)
    {
        rob.errorG[id] = rob.errorG[id] * 0.8 + (d)*time * 0.001;
        errorD = (d - rob.distanceA[id]) / (time * 0.001);

        rob.distanceA[id] = d;
        rob.before[id] = now;

        prop += errorD * kd + rob.errorG[id] * ki;
        std::cout << "prop: " << prop << std::endl;
        right *= prop;
        left *= prop;
    }

    right = map(right, -s1, s1, -s2, s2);
    left = map(left, -s1, s1, -s2, s2);

    if (right > -min && right < min)
        right = right < 0 ? -min : min;

    if (left > -min && left < min)
        left = left < 0 ? -min : min;

    return std::make_pair(right, left);
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