#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "Position.h"
#include "Map.h"

class Pedestrian {
public:
    Pedestrian(const Position& pos, bool movesVertically, int maxMoney);

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

    void GenerateMoney(int maxAmount);
};

#endif