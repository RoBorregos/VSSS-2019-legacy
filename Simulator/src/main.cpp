#include <cmath>
#include <Communications/CommandSender.h>
#include <Communications/DebugSender.h>
#include <Communications/StateReceiver.h>
#include <future>
#include <math.h>
#include <stack>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

#include "../../libraries/a_star/a_star.h"

#define MAX_MOTOR_VELOCITY 10
#define SAFE_ZONE 5
#define TEAM_BLUE 0
#define TEAM_YELLOW 1

#define BOTTOM_LEFT_CORNER std::pair<int, int>(0, 130)
#define BOTTOM_RIGHT_CORNER std::pair<int, int>(170, 130)
#define TOP_LEFT_CORNER std::pair<int, int>(0, 0)
#define TOP_RIGHT_CORNER std::pair<int, int>(170, 0)

/*
    Angle:
                270
                 ^
                 |
       180  <-       ->  0
                 |
                 v
                90

    Coordinate of important points

    Crosses:
        Down right: 122, 104
        Up right: 122, 25
        Up left: 48, 25
        Down left: 48, 104

    Goal:
        Right goal: 
            Down: 155, 81
            Up: 155, 49
        Left goal: 
            Down: 15, 81
            Up: 15, 49
    
    Center: 85, 65

*/

vss::IStateReceiver *state_receiver;
vss::ICommandSender *command_sender;
vss::IDebugSender *debug_sender;

void printData() {
    auto states = state_receiver->receiveState(
      vss::FieldTransformationType::None);

    std::cout << states << std::endl;
}

int main(int argc, char** argv){
    state_receiver = new vss::StateReceiver();
    command_sender = new vss::CommandSender();
    debug_sender = new vss::DebugSender();

    state_receiver->createSocket();
    command_sender->createSocket(vss::TeamType::Yellow);
    debug_sender->createSocket(vss::TeamType::Yellow);


    std::vector<std::pair<int, int>> corners;
    corners.push_back(TOP_LEFT_CORNER);
    corners.push_back(TOP_RIGHT_CORNER);
    corners.push_back(BOTTOM_RIGHT_CORNER);
    corners.push_back(BOTTOM_LEFT_CORNER);

    a_star::AStar a_star_map;
    while (true) {
        auto states = state_receiver->receiveState(
          vss::FieldTransformationType::None);
        std::cout << states << std::endl;
        std::tuple<a_star::Robot, int, int> goalie_coordinate = std::make_tuple(
          a_star::Robot::GOALIE, states.teamYellow.at(0).x,
          states.teamYellow.at(0).y);
        std::tuple<a_star::Robot, int, int> defender_coordinate = 
          std::make_tuple(a_star::Robot::DEFENDER, states.teamYellow.at(1).x, 
          states.teamYellow.at(1).y);
        std::tuple<a_star::Robot, int, int> mid_fielder_coordinate = 
          std::make_tuple(a_star::Robot::MID_FIELDER, 
          states.teamYellow.at(2).x, states.teamYellow.at(2).y);
        std::pair<int, int> ball_coordinate = std::make_pair(
          states.ball.x, states.ball.y);
        std::tuple<a_star::Robot, int, int> enemy_goalie_coordinate = 
          std::make_tuple(a_star::Robot::UNDEFINED, states.teamBlue.at(0).x, 
          states.teamBlue.at(0).y);
        std::tuple<a_star::Robot, int, int> enemy_defender_coordinate = 
          std::make_tuple(a_star::Robot::UNDEFINED, states.teamBlue.at(1).x, 
          states.teamBlue.at(1).y);
        std::tuple<a_star::Robot, int, int> enemy_mid_fielder_coordinate = 
          std::make_tuple(a_star::Robot::UNDEFINED, states.teamBlue.at(2).x, 
          states.teamBlue.at(2).y);

        a_star_map.CreateMap(std::vector<std::tuple<a_star::Robot, int, int>> {
          goalie_coordinate, defender_coordinate, mid_fielder_coordinate, 
          enemy_goalie_coordinate, enemy_defender_coordinate, 
          enemy_mid_fielder_coordinate}, corners, 
          SAFE_ZONE);

        vss::Debug debug;
        bool founded_path = a_star_map.AStarSearch(a_star::Robot::GOALIE,
          std::make_pair(std::get<1>(goalie_coordinate), 
          std::get<2>(goalie_coordinate)), ball_coordinate);
        if (founded_path) {
            vss::Path debug_path;
            auto path = a_star_map.GetPath(std::make_pair(
              std::get<1>(goalie_coordinate), std::get<2>(goalie_coordinate)), 
              ball_coordinate);
            while (!path.empty()) {
                auto next_coordinate = path.top();
                debug_path.points.push_back(vss::Point(next_coordinate.first,
                    next_coordinate.second));
                path.pop();
            }
            debug.paths.push_back(debug_path);
        } else {
            std::cout << "Path not found" << std::endl;
        }
        founded_path = a_star_map.AStarSearch(a_star::Robot::DEFENDER,
          std::make_pair(std::get<1>(defender_coordinate), 
          std::get<2>(defender_coordinate)), ball_coordinate);
        if (founded_path) {
            vss::Path debug_path;
            auto path = a_star_map.GetPath(std::make_pair(
              std::get<1>(defender_coordinate), 
              std::get<2>(defender_coordinate)), ball_coordinate);
            while (!path.empty()) {
                auto next_coordinate = path.top();
                debug_path.points.push_back(vss::Point(next_coordinate.first,
                    next_coordinate.second));
                path.pop();
            }
            debug.paths.push_back(debug_path);
        } else {
            std::cout << "Path not found" << std::endl;
        }
        founded_path = a_star_map.AStarSearch(a_star::Robot::MID_FIELDER,
          std::make_pair(std::get<1>(mid_fielder_coordinate), 
          std::get<2>(mid_fielder_coordinate)), ball_coordinate);
        if (founded_path) {
            vss::Path debug_path;
            auto path = a_star_map.GetPath(std::make_pair(
              std::get<1>(mid_fielder_coordinate), 
              std::get<2>(mid_fielder_coordinate)), 
              ball_coordinate);
            while (!path.empty()) {
                auto next_coordinate = path.top();
                debug_path.points.push_back(vss::Point(next_coordinate.first,
                    next_coordinate.second));
                path.pop();
            }
            debug.paths.push_back(debug_path);
        } else {
            std::cout << "Path not found" << std::endl;
        }

        vss::Command command;
        command.commands.push_back(vss::WheelsCommand(MAX_MOTOR_VELOCITY, 
          MAX_MOTOR_VELOCITY));
        command.commands.push_back(vss::WheelsCommand(MAX_MOTOR_VELOCITY, 
          MAX_MOTOR_VELOCITY));
        command.commands.push_back(vss::WheelsCommand(MAX_MOTOR_VELOCITY, 
          MAX_MOTOR_VELOCITY));
        command_sender->sendCommand(command);
        debug_sender->sendDebug(debug);
    }

    return 0;
}