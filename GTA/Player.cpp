#include "Player.h"
#include "Pedestrian.h"
#include "Map.h"
#include "IslandConfig.h"
#include <cstdlib>

Player::Player(const Map& map) 
{
    position.x = std::max(1, std::min(map.GetWidth() - 2, map.GetWidth() / 2));
    position.y = std::max(1, std::min(map.GetHeight() - 2, map.GetHeight() / 2));

    currentDirection = Direction::RIGHT;
}

void Player::Move(Direction direction, Map& map, std::vector<Island>& island)
{
    Position newPos = position;

    switch (direction) 
    {
    case Direction::UP: 
        newPos.y--; 
        break;
    case Direction::DOWN: 
        newPos.y++; 
        break;
    case Direction::LEFT: 
        newPos.x--; 
        break;
    case Direction::RIGHT: 
        newPos.x++; 
        break;
    }

    if (!map.IsPositionValid(newPos)) 
    {
        return;
    }

    Cell targetCell = map.GetCell(newPos);

    if (targetCell.type == CellType::MONEY) 
    {
        money += targetCell.moneyInCell;
        Cell emptyCell;
        emptyCell.type = CellType::EMPTY;
        map.SetCell(newPos, emptyCell);
    }

    if (targetCell.type == CellType::BRIDGE) 
    {
        int islandWidth = map.GetWidth() / 3;

        if (newPos.x == islandWidth && money >= island.at(0).GetMoneyToNext())
        {
            money -= island.at(0).GetMoneyToNext();
        }
        else if (newPos.x == islandWidth * 2 && money >= island.at(1).GetMoneyToNext())
        {
            money -= island.at(1).GetMoneyToNext();
        }
        else 
        {
            return;
        }
    }

    position = newPos;
}

void Player::Attack(std::vector<Pedestrian>& pedestrians, Map& map, int maxMoney)
{
    std::vector<Pedestrian> newPedestrians;

    for (auto it = pedestrians.begin(); it != pedestrians.end();)
    {
        if (!it->IsAlive())
        {
            ++it;
            continue;
        }

        Position pedPos = it->GetPosition();

        if (abs(pedPos.x - position.x) + abs(pedPos.y - position.y) == 1)
        {
            it->TakeDamage(attackPower);
            if (!it->IsAlive())
            {
                Cell moneyCell;
                moneyCell.type = CellType::MONEY;
                moneyCell.moneyInCell = it->GetMoney();
                map.SetCell(pedPos, moneyCell);

                newPedestrians.push_back(GenerateNewPedestrian(map, maxMoney));
                it = pedestrians.erase(it);
                continue;
            }
            else if (it->IsAggressive())
            {
                this->TakeDamage(it->GetAttackPower());

                if (!this->IsAlive())
                {
					system("cls");
                    std::cout << "CJ ha muerto. Game Over.\n";
                 /*  Sleep(3000);
                    exit(0); // Temporal hasta tener GameState::GameOver*/
                }
            }
        }

        ++it;
    }

    pedestrians.insert(pedestrians.end(), newPedestrians.begin(), newPedestrians.end());
}
