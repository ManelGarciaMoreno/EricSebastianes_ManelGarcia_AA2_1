#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "Player.h"
#include "Map.h"
#include "IslandConfig.h"
#include <vector>

class GameLoop 
{
public:
    static const int FPS = 60;

    GameLoop(const GameConfig& config);
    void Run();

private:
    Player player;
    Map map;
    std::vector<Island> islands;
    std::vector<Pedestrian> allPedestrians;

    void ProcessInput();
    void Update();
    void Render() const;
    void SpawnPedestrians();
};

#endif