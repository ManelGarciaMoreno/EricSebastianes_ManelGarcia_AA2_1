#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "Position.h"
#include "Map.h"
#include "Windows.h"

class Player;

class Pedestrian 
{
public:
    Pedestrian(const Position& pos, bool movesVertically, int maxMoney, int health, int attackPower, bool isAggressive);

    void GenerateMoney(int maxAmount);

    static Pedestrian GenerateNewPedestrian(const Map& map, int maxMoney);

    void Move(const Map& map, const Position& playerPos);

    void TryAttackPlayer(Player& player);

    // void TakeDamage(int damage);
    // 
    // void AttackPlayer(Player& player);

    bool IsAggressive() const 
    { 
        return isAggressive; 
    }

    int GetHealth() const 
    { 
        return health; 
    }
    int GetAttackPower() const 
    { 
        return attackPower; 
    }

    Position GetPosition() const
    {
        return position;
    }

    int GetMoney() const
    {
        return money;
    }

    bool IsAlive() const 
    { 
        return isAlive; 
    }

    void TakeDamage(int damage)
    {
        health -= damage;
        if (health <= 0) isAlive = false;
    }




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