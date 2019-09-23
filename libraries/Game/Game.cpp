#include "Game.h"
#include "Control.h"

Game::Game()
{
    for (uint8_t i = 0; i < 3; i++)
    {
        ourRobots.push_back(new Robot());
    }

    for (uint8_t i = 0; i < 3; i++)
    {
        theirRobots.push_back(new Robot());
    }
    timeRemaing = 0;
    half = 1;
}

void Game::initializeGame()
{
    // vision.initialize();
    // strategy.define("normal");
}

void Game::play()
{
    while (true)
    {
        // vision.update();
        // strategy.defineFinalPoints();
        // pathPlanning.defineInmediatePoints();
        Control::triggerActions();
    }
}