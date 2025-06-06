#include "GameLoop.h"
#include "GameConfig.h"
#include <Windows.h>
#include <iostream>
#include <cstdlib>

bool GameLoop::gameRunning = true;

GameLoop::GameLoop(const GameConfig& config) : map(config.mapWidth, config.mapHeight), player(map, config.cjAttackPower, config.cjHealth)
{
    map.Initialize(config);

    islands.emplace_back(config.pedestriansLosSantos, config.maxMoneyLosSantos, config.moneyToSanFierro, config.pedestrianHealthLosSantos, config.pedestrianAttackLosSantos);
    islands.emplace_back(config.pedestriansSanFierro, config.maxMoneySanFierro, config.moneyToLasVenturas, config.pedestrianHealthSanFierro, config.pedestrianAttackSanFierro);
    islands.emplace_back(config.pedestriansLasVenturas, config.maxMoneySanFierro, 0, config.pedestrianHealthLasVenturas, config.pedestrianAttackLasVenturas);

    SpawnPedestrians();
}

void GameLoop::ChangeState(int state) { gameState = state; }


void GameLoop::Run()
{
    while (gameRunning)
    {
        ProcessInput();
        Update();
        Render();
        Sleep(1000 / FPS);
    }
}

void GameLoop::ProcessInput()
{
    if (gameState == 1)
    {
        if (GetAsyncKeyState(VK_UP)) playButton = true;
        if (GetAsyncKeyState(VK_DOWN)) playButton = false;

        if ((GetAsyncKeyState(VK_SPACE)) && !playButton)
        {
            exit(0);
        }
        else if (GetAsyncKeyState(VK_SPACE))
        {
            ChangeState(2);
        }
    }
    else if (gameState == 2) 
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
}

void GameLoop::Update() 
{
    allPedestrians.erase(std::remove_if(allPedestrians.begin(), allPedestrians.end(), [](const Pedestrian& p) { return !p.IsAlive(); }), allPedestrians.end());
    for (auto& pedestrian : allPedestrians)
    {
        pedestrian.Move(map, player.GetPosition());
        pedestrian.TryAttackPlayer(player);
    }

    if (!player.IsAlive() && gameState == 2) {
        ExitGame();
        return;
    }
}

void GameLoop::Render()
{
    system("cls");

    if (gameState == 0) 
    {
        std::cout << R"(
            =========================
            Gran Thef Auto: Enti City
            =========================
        )" << '\n';

        Sleep(3000);
        ChangeState(1);
    } 
    else if (gameState == 1) 
    {
        if (playButton == true) {
            std::cout << R"(
                ========================
                ||        PLAY        ||
                ========================
                ------------------------
                |         EXIT         |
                ------------------------
            )" << '\n';
        }
        else 
        {
            std::cout << R"(
                ------------------------
                |         PLAY         |
                ------------------------
                ========================
                ||        EXIT        ||
                ========================
            )" << '\n';
        }
    }
    else if (gameState == 2)
    {
        const int viewRadius = 10;
        Position center = player.GetPosition();

        for (int y = (std::max)(0, center.y - viewRadius); y <= (std::min)(map.GetHeight() - 1, center.y + viewRadius); y++)
        {
            for (int x = (std::max)(0, center.x - viewRadius); x <= (std::min)(map.GetWidth() - 1, center.x + viewRadius); x++)
            {
                Position pos = 
                {
                    x,
                    y
                };

                Cell cell = map.GetCell(pos);

                if (pos == player.GetPosition())
                {
                    switch (player.GetDirection())
                    {
                    case Player::Direction::UP:
                        std::cout << '^';
                        break;
                    case Player::Direction::DOWN:
                        std::cout << 'v';
                        break;
                    case Player::Direction::LEFT:
                        std::cout << '<';
                        break;
                    case Player::Direction::RIGHT:
                        std::cout << '>';
                        break;
                    }
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
                validPosition = (cell.type != CellType::WALL);

                for (const auto& ped : allPedestrians)
                {
                    if (ped.GetPosition() == pos)
                    {
                        validPosition = false;
                        break;
                    }
                }

                attempts++;
            }

            if (validPosition)
            {
                bool movesVertically = rand() % 2 == 0;
                bool isAggressive = rand() % 2 == 0;

                Pedestrian pedestrian(
                    pos,
                    movesVertically,
                    island.GetMaxMoney(),
                    island.GetPedestrianHealth(),
                    island.GetPedestrianAttack(),
                    isAggressive
                );

                allPedestrians.push_back(pedestrian);
            }
        }
    }
}