#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "Player.h"
#include "Position.h"
#include "Map.h"

class Pedestrian 
{
public:
    Pedestrian(const Position& pos, bool movesVertically, int maxMoney, int health, int attackPower, bool isAggressive);
    void GenerateMoney(int maxAmount);
    void TakeDamage(int damage);
    void AttackPlayer(Player& player);
    bool IsAggressive() const { return isAggressive; }
    int GetHealth() const { return health; }

    void Move(const Map& map, const Position& playerPos);
    bool IsAlive() const 
    { 
        return isAlive; 
    }

    void Kill() 
    { 
        isAlive = false; 
    }

    Position GetPosition() const 
    { 
        return position; 
    }

    int GetMoney() const 
    { 
        return money; 
    }

private:
    Position position;
    int money;
    bool isAlive = true;
    bool movesVertically;
    int health;
    int attackPower;
    bool isAggressive;
};
#endif