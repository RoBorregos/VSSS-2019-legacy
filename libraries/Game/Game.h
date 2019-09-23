#if !defined(GAMEH_H)
#define GAMEH_H

#include "Strategy.h"
#include "Robot.h"
#include "Control.h"
#include <vector>

class Game
{
public:
    Game();
    void initializeGame();
    void play();

private:
    Strategy strategy;
    std::vector<Robot*> ourRobots;
    std::vector<Robot*> theirRobots;
    Figure* ball;

    float timeRemaing;
    uint8_t half;
};

#endif // GAMEH_H