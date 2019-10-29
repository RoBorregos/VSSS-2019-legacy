#if !defined(GAMEH_H)
#define GAMEH_H

#include "Strategy.h"
#include "Robot.h"
#include <vector>

using namespace std;

class Game
{
public:
    Game();
    void initializeGame();

private:
    Strategy strategy;
    vector<Robot*> ourRobots;
    vector<Robot*> theirRobots;
    Figure* ball;

    float timeRemaing;
};

#endif // GAMEH_H