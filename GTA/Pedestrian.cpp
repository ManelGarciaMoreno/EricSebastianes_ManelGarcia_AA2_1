#include "Pedestrian.h"
#include "Player.h"
#include <cstdlib>

Pedestrian::Pedestrian(const Position& pos, bool movesVertically, int maxMoney, int hp, int atk, bool aggressive)
    : position(pos), movesVertically(movesVertically), health(hp), attackPower(atk), isAggressive(aggressive)
{
    isAlive = true;
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

        if (!player.IsAlive())
        {
			system("cls");
            std::cout << "CJ ha muerto. Game Over.\n";
            exit(0); // Reemplazar por cambio a GameState::GameOver
        }
    }
}