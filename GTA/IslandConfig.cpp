#include "IslandConfig.h"

Island::Island(int maxPedestrians, int maxMoney, int moneyToNext) : maxPedestrians(maxPedestrians), maxMoney(maxMoney), moneyToNext(moneyToNext) {}

void Island::AddPedestrian(const Pedestrian& pedestrian) 
{
    if (pedestrians.size() < maxPedestrians) 
    {
        pedestrians.push_back(pedestrian);
    }
}