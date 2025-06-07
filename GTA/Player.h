#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "Direction.h"
#include "IslandConfig.h"
#include "Map.h"
#include <vector>
#include <iostream>

class Car;
class Island;
class Pedestrian;

class Player
{
public:
    int health;
    int attackPower;
    bool isInCar = false;
    using Direction = ::Direction;

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

    void SetPosition(const Position& pos) { position = pos; }

    Car* currentCar = nullptr;

    void SetCurrentCar(Car* car) { currentCar = car; }

    Car* GetCurrentCar() const
    {
        return currentCar; 
    }

    void EnterCar(Car* car);
    void ExitCar();

private:
    Position position;
    Direction currentDirection;
    int money = 0;
};
#endif