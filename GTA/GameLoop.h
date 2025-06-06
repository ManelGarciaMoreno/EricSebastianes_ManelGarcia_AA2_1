#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "Player.h"
#include "Map.h"
#include "IslandConfig.h"
#include <vector>
#include <iostream>

class GameLoop
{
public:
    static const int FPS = 60;
    static bool gameRunning; 

    GameLoop(const GameConfig& config);
    void Run();
    void ChangeState(int state);

    static void ExitGame()
    {
        gameRunning = false;
        system("cls");
        std::cout << "Game Over" << std::endl;
    }

private:
    Player player;
    Map map;
    std::vector<Island> islands;
    std::vector<Pedestrian> allPedestrians;

    int gameState = 0;
    bool playButton = true;

    void ProcessInput();
    void Update();
    void Render();
    void SpawnPedestrians();
};

#endif