#include "GameLoop.h"
#include "GameConfig.h"
#include <Windows.h>
#include <iostream>
#include <cstdlib>

GameLoop::GameLoop(const GameConfig& config) : map(config.mapWidth, config.mapHeight), player(map)
{
    map.Initialize(config);

    islands.emplace_back(config.pedestriansLosSantos, config.maxMoneyLosSantos, config.maxMoneyLosSantos);
    islands.emplace_back(config.pedestriansSanFierro, config.maxMoneySanFierro, config.maxMoneySanFierro);

    SpawnPedestrians();
}

void GameLoop::Run() 
{
    while (true) 
    {
        if (GetAsyncKeyState(VK_ESCAPE)) 
        {
            system("cls");
            std::cout << "Game exited" << std::endl;
            break;
        }

        ProcessInput();
        Update();
        Render();

        Sleep(1000 / FPS);
    }
}

void GameLoop::ProcessInput() 
{
    if (GetAsyncKeyState(VK_UP)) player.Move(Player::Direction::UP, map, islands);
    if (GetAsyncKeyState(VK_DOWN)) player.Move(Player::Direction::DOWN, map, islands);
    if (GetAsyncKeyState(VK_LEFT)) player.Move(Player::Direction::LEFT, map, islands);
    if (GetAsyncKeyState(VK_RIGHT)) player.Move(Player::Direction::RIGHT, map, islands);

    if (GetAsyncKeyState(VK_SPACE))
    {
        int currentIslandIndex = player.GetPosition().x / (map.GetWidth() / 3);
        int maxMoney = 0;

        if (currentIslandIndex < islands.size()) {
            maxMoney = islands[currentIslandIndex].GetMaxMoney();
        }

        player.Attack(allPedestrians, map, maxMoney);
    }
}

void GameLoop::Update() 
{
    allPedestrians.erase(std::remove_if(allPedestrians.begin(), allPedestrians.end(), [](const Pedestrian& p) { return !p.IsAlive(); }), allPedestrians.end());
    
    for (auto& pedestrian : allPedestrians) 
    {
        pedestrian.Move(map, player.GetPosition());
    }
}

void GameLoop::Render() const
{
    system("cls");

    const int viewRadius = 10;
    Position center = player.GetPosition();

    for (int y = (std::max)(0, center.y - viewRadius); y <= (std::min)(map.GetHeight() - 1, center.y + viewRadius); y++)
    {
        for (int x = (std::max)(0, center.x - viewRadius); x <= (std::min)(map.GetWidth() - 1, center.x + viewRadius); x++)
        {
            Position pos 
            { 
                x, 
                y 
            };

            Cell cell = map.GetCell(pos);

            if (pos == player.GetPosition())
            {
                std::cout << 'P';
            }
            else
            {
                bool pedestrianFound = false;
                for (const auto& pedestrian : allPedestrians)
                {
                    if (pedestrian.GetPosition() == pos && pedestrian.IsAlive())
                    {
                        std::cout << 'p';
                        pedestrianFound = true;
                        break;
                    }
                }

                if (!pedestrianFound)
                {
                    std::cout << cell.ToChar();
                }
            }
        }
        std::cout << "\n";
    }

    std::cout << "Dinero: " << player.GetMoney() << std::endl;
}

void GameLoop::SpawnPedestrians()
{
    allPedestrians.clear();

    for (size_t i = 0; i < islands.size(); i++)
    {
        const Island& island = islands[i];

        for (int j = 0; j < island.GetMaxPedestrians(); j++)
        {
            Position pos;
            int attempts = 0;
            bool validPosition = false;

            while (!validPosition && attempts < 100)
            {
                pos.x = rand() % (map.GetWidth() / 3) + (i * map.GetWidth() / 3) + 1;
                pos.y = rand() % (map.GetHeight() - 2) + 1;

                Cell cell = map.GetCell(pos);
                if (cell.type != CellType::WALL)
                {
                    validPosition = true;

                    for (const auto& ped : allPedestrians)
                    {
                        if (ped.GetPosition() == pos)
                        {
                            validPosition = false;
                            break;
                        }
                    }
                }

                attempts++;
            }

            if (validPosition)
            {
                bool movesVertically = rand() % 2 == 0;
                Pedestrian pedestrian(pos, movesVertically, island.GetMaxMoney());
                allPedestrians.push_back(pedestrian);
            }
        }
    }
}