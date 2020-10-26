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
#include "SimulatorBridge/SimulatorBridge.h"
using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSenderY;
IDebugSender *debugSenderY;
ICommandSender *commandSenderB;
IDebugSender *debugSenderB;

State state;


std::vector<std::pair<double, double>> velocitiesYellow;
std::vector<std::pair<double, double>> velocitiesBlue;

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

  SimulatorBridge simulador;

  while (true)
  {
    vss::Debug debug;
    vss::Command command;
    //necesario para "comunicacion"
    velocitiesYellow = {std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0)};
    velocitiesBlue = {std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0)};
    
    // vision (ish)
    state = stateReceiver->receiveState(FieldTransformationType::None);
    simulador.updateShapes(state.teamYellow);


    // TODO estrategia
    // estrategia rapida de prueba (ir al balon)
    for(int i=0; i<3; i++){
    simulador.setFinalPos(i,state.ball.x, state.ball.y);
    simulador.setFinalAngle(i,simulador.myControl.generateGenericFinalAngle(i));
    }

    //debug poses deseados de los robots aliados
    for(int i=0; i<3; i++)
    debug.finalPoses.push_back(Pose(simulador.allies[i].currentPos.x, simulador.allies[i].currentPos.y, simulador.allies[i].finalOri));
    
    //TODO path planning

    //control 
    for(int i=0; i<3; i++){
    double r = 0, l = 0;
    std::cout<<"Control "<<i<<std::endl;
    simulador.myControl.move(i,r,l);
    std::cout<<r<<" "<<l<<std::endl;
    velocitiesYellow[i] = std::make_pair(r,l);
    }
    //comunicacion
    send_commands(velocitiesYellow, velocitiesBlue);

    debugSenderY->sendDebug(debug);
  }
  return 0;
}
