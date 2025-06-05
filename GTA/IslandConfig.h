#ifndef ISLAND_H
#define ISLAND_H

#include "Pedestrian.h"
#include <vector>

class Island 
{
public:
    Island(int maxPedestrians, int maxMoney, int moneyToNext);

    void AddPedestrian(const Pedestrian& pedestrian);
    const std::vector<Pedestrian>& GetPedestrians() const 
    { 
        return pedestrians; 
    }

    int GetMaxPedestrians() const 
    { 
        return maxPedestrians; 
    }

    int GetMaxMoney() const 
    { 
        return maxMoney; 
    }

    int GetMoneyToNext() const
    {
        return maxMoney;
    }

private:
    std::vector<Pedestrian> pedestrians;
    int maxPedestrians;
    int maxMoney;
    int moneyToNext;

};

#endif