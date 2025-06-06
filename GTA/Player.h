#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "Map.h"
#include "Pedestrian.h"
#include "IslandConfig.h"
#include <vector>
#include <iostream>

class Player 
{
public:
    int health;
    int attackPower;
    bool isInCar;

    enum class Direction { 
        UP, 
        DOWN, 
        LEFT, 
        RIGHT 
    };

    Player(const Map& map);

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

private:
    Position position;
    Direction currentDirection;
    int money = 0;

    //Pedestrian GenerateNewPedestrian(const Map& map, int maxMoney) const;
};

#endif