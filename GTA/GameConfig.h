#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>

struct GameConfig 
{
    int mapWidth = 0;
    int mapHeight = 0;

    int pedestriansLosSantos = 0;
    int pedestriansSanFierro = 0;
    int pedestriansLasVenturas = 0;

    int moneyToSanFierro = 0;
    int moneyToLasVenturas = 0;

    int maxMoneyLosSantos = 0;
    int maxMoneySanFierro = 0;

    bool LoadFromFile(const std::string& filename);
};

#endif