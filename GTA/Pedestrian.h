#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "Player.h"
#include "Position.h"
#include "Map.h"
#include "Windows.h"

class Pedestrian 
{
public:
    Pedestrian(const Position& pos, bool movesVertically, int maxMoney, int health, int attackPower, bool isAggressive);
    void GenerateMoney(int maxAmount);
    void TakeDamage(int damage);
    bool IsAggressive() const { return isAggressive; }
    int GetHealth() const { return health; }
    int GetAttackPower() const { return attackPower; }

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

    void TakeDamage(int damage)
    {
        health -= damage;
        if (health <= 0) isAlive = false;
    }
    void TryAttackPlayer(Player& player);

private:
    Position position;
    int money;
    bool isAlive = true;
    bool movesVertically;
    int health;
    int attackPower;
    bool isAggressive;
    DWORD lastAttackTime = 0;
};
#endif