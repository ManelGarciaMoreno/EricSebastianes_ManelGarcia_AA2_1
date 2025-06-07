#pragma once
#ifndef ISLAND_H
#define ISLAND_H

#include "Pedestrian.h"
#include "GameConfig.h"
#include <vector>

class Island 
{
public:
    Island(int maxPedestrians, int maxMoney, int moneyToNext, int pedHealth, int pedAttack);

    void AddPedestrian(const Pedestrian& pedestrian);
    int GetPedestrianHealth() const { return pedestrianHealth; };
    int GetPedestrianAttack() const { return pedestrianAttack; };

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
        return moneyToNext;
    }

private:
    int pedestrianHealth;
    int pedestrianAttack;

    std::vector<Pedestrian> pedestrians;
    int maxPedestrians;
    int maxMoney;
    int moneyToNext;

};

#endif