#include "Pedestrian.h"
#include <cstdlib>

Pedestrian::Pedestrian(const Position& pos, bool movesVertically, int maxMoney) : position(pos), movesVertically(movesVertically) 
{
    GenerateMoney(maxMoney);
}

void Pedestrian::Move(const Map& map, const Position& playerPos)
{
    if (!isAlive) return;

    if (abs(position.x - playerPos.x) <= 1 && abs(position.y - playerPos.y) <= 1)
    {
        return;
    }

    Position newPos = position;
    int direction = (rand() % 2 == 0) ? 1 : -1;

    if (movesVertically)
    {
        newPos.y += direction;
    }
    else
    {
        newPos.x += direction;
    }

    if (map.IsPositionValid(newPos))
    {
        position = newPos;
    }
    else
    {
        movesVertically = !movesVertically;
    }
}

void Pedestrian::GenerateMoney(int maxAmount) 
{
    money = rand() % maxAmount + 1;
}