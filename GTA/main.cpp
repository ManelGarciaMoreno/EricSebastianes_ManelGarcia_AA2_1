#include "GameLoop.h"
#include "GameConfig.h"
#include <iostream>


int main() 
{
    GameConfig config;

    if (!config.LoadFromFile("config.txt")) 
    {
        std::cout << "Failed to load config file" << std::endl;
        return 1;
    }

    GameLoop game(config);
    game.Run();
}