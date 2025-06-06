#include "Pedestrian.h"
#include "Player.h"
#include <cstdlib>
#include <iostream>

Pedestrian::Pedestrian(const Position& pos, bool movesVertically, int maxMoney, int hp, int atk, bool aggressive) : position(pos), movesVertically(movesVertically), health(hp), attackPower(atk), isAggressive(aggressive)
{
    isAlive = true;
    GenerateMoney(maxMoney);
}

Pedestrian Pedestrian::GenerateNewPedestrian(const Map& map, int maxMoney)
{
    Position newPos;

    newPos.x = rand() % (map.GetWidth() / 3) + 1;
    newPos.y = rand() % (map.GetHeight() - 2) + 1;

    bool movesVertically = rand() % 2 == 0;
    bool isAggressive = rand() % 2 == 0;
    return Pedestrian(newPos, movesVertically, maxMoney, 1, 1, isAggressive);
}

void Pedestrian::TryAttackPlayer(Player& player)
{
    if (!isAlive || !isAggressive)
        return;

    Position playerPos = player.GetPosition();
    if (abs(position.x - playerPos.x) + abs(position.y - playerPos.y) > 1)
        return;

    DWORD currentTime = GetTickCount();

    if (currentTime - lastAttackTime >= 1000)
    {
        player.TakeDamage(attackPower);
        lastAttackTime = currentTime;
    }
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