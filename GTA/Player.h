#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "Map.h"
#include "IslandConfig.h"
#include <vector>
#include <iostream>

class Pedestrian;

class Player
{
public:
    int health;
    int attackPower;
    bool isInCar = false;

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Player(const Map& map, const int& attack, const int& health);

    void Move(Direction direction, Map& map, std::vector<Island>& island);
    void Attack(std::vector<Pedestrian>& pedestrians, Map& map, int maxMoney);

    Position GetPosition() const
    {
        return position;
    }

    Direction GetDirection() const
    {
        return currentDirection;
    }

    int GetMoney() const
    {
        return money;
    } 
    int GetHealth() const
    {
        return health;
    }

    void AddMoney(int amount)
    {
        money += amount;
    }

    void TakeDamage(int dmg)
    {
        health -= dmg;
    }

    bool IsAlive() const
    {
        return health > 0;
    }

    bool IsInCar() const { return isInCar; }

    void EnterCar() { isInCar = true; }

    void ExitCar() { isInCar = false; }

    void SetPosition(const Position& pos) { position = pos; }

private:
    Position position;
    Direction currentDirection;
    int money = 0;
};

#endif