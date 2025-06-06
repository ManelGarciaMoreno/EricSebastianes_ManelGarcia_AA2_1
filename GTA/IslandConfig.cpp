#include "IslandConfig.h"

Island::Island(int maxPedestrians, int maxMoney, int moneyToNext, int pedHealth, int pedAttack) : maxPedestrians(maxPedestrians), maxMoney(maxMoney), moneyToNext(moneyToNext), pedestrianHealth(pedHealth), pedestrianAttack(pedAttack) {}

void Island::AddPedestrian(const Pedestrian& pedestrian) 
{
    if (pedestrians.size() < maxPedestrians) 
    {
        pedestrians.push_back(pedestrian);
    }
}