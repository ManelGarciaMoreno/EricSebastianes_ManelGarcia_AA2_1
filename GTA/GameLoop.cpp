#include "GameLoop.h"
#include "GameConfig.h"
#include "BigSmoke.h"
#include <Windows.h>
#include <Winuser.h>
#include <iostream>
#include <cstdlib>
#include "Player.h"

#define VK_E 0x45
#define VK_E_MIN 0x65

bool GameLoop::gameRunning = true;
bool carFound = false;
BigSmoke* finalBoss = nullptr;

GameLoop::GameLoop(const GameConfig& config) : map(config.mapWidth, config.mapHeight), player(map, config.cjAttackPower, config.cjHealth)
{
    map.Initialize(config);

    islands.emplace_back(config.pedestriansLosSantos, config.maxMoneyLosSantos, config.moneyToSanFierro, config.pedestrianHealthLosSantos, config.pedestrianAttackLosSantos);
    islands.emplace_back(config.pedestriansSanFierro, config.maxMoneySanFierro, config.moneyToLasVenturas, config.pedestrianHealthSanFierro, config.pedestrianAttackSanFierro);
    islands.emplace_back(config.pedestriansLasVenturas, config.maxMoneySanFierro, 0, config.pedestrianHealthLasVenturas, config.pedestrianAttackLasVenturas);

    SpawnPedestrians();
    SpawnCars();
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
        if (GetAsyncKeyState(VK_UP))
        {
            if (player.IsInCar() && player.GetCurrentCar() != nullptr)
                player.GetCurrentCar()->Move(Player::Direction::UP, map);
            else
                player.Move(Player::Direction::UP, map, islands);
        }
        if (GetAsyncKeyState(VK_DOWN))
        {
            if (player.IsInCar() && player.GetCurrentCar() != nullptr)
                player.GetCurrentCar()->Move(Player::Direction::DOWN, map);
            else
                player.Move(Player::Direction::DOWN, map, islands);
        }
        if (GetAsyncKeyState(VK_LEFT))
        {
            if (player.IsInCar() && player.GetCurrentCar() != nullptr)
                player.GetCurrentCar()->Move(Player::Direction::LEFT, map);
            else
                player.Move(Player::Direction::LEFT, map, islands);
        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            if (player.IsInCar() && player.GetCurrentCar() != nullptr)
                player.GetCurrentCar()->Move(Player::Direction::RIGHT, map);
            else
                player.Move(Player::Direction::RIGHT, map, islands);
        }

        if (GetAsyncKeyState(VK_SPACE))
        {
            int currentIslandIndex = player.GetPosition().x / (map.GetWidth() / 3);
            int maxMoney = 0;

            if (currentIslandIndex < islands.size()) {
                maxMoney = islands[currentIslandIndex].GetMaxMoney();
            }

            player.Attack(allPedestrians, map, maxMoney);
        }

        if ((GetAsyncKeyState(VK_E) || GetAsyncKeyState(VK_E_MIN)))
        {
            if(!player.IsInCar())
            {
                for (auto& car : allCars)
                {
                    if (car.GetPosition() == player.GetPosition() && !car.IsOccupied())
                    {
                        player.EnterCar(&car);
                        car.SetOccupied(true);
                        break;
                    }
                }
            }
            else
            {
                player.ExitCar();                
            }

            Sleep(150);
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

    if (player.IsInCar() && player.GetCurrentCar() != nullptr)
    {
        player.SetPosition(player.GetCurrentCar()->GetPosition());
    }

    if (player.IsInCar() && player.GetCurrentCar() != nullptr)
    {
        Position carPos = player.GetCurrentCar()->GetPosition();

        for (auto it = allPedestrians.begin(); it != allPedestrians.end(); )
        {
            if (it->IsAlive() && it->GetPosition() == carPos)
            {
                it->Kill();

                Cell moneyCell;
                moneyCell.type = CellType::MONEY;
                moneyCell.moneyInCell = it->GetMoney();
                map.SetCell(carPos, moneyCell);

                std::cout << "¡Peatón atropellado!\n";
                it = allPedestrians.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

}

void GameLoop::SpawnCars()
{
    int numCars = 6;
    allCars.clear();

    for (int i = 0; i < numCars; ++i)
    {
        Position pos;
        bool valid = false;
        int attempts = 0;

        while (!valid && attempts < 100)
        {
            int islandIndex = rand() % 3;
            int islandWidth = map.GetWidth() / 3;
            int startX = islandIndex * islandWidth + 1;
            int endX = (islandIndex + 1) * islandWidth - 2;

            pos.x = rand() % (endX - startX + 1) + startX;
            pos.y = rand() % (map.GetHeight() - 2) + 1;

            Cell cell = map.GetCell(pos);

            // No colocar coche en casillas ocupadas por algo
            if (cell.type == CellType::WALL || cell.type == CellType::MONEY)
            {
                attempts++;
                continue;
            }

            bool conflict = false;

            for (const auto& ped : allPedestrians)
                if (ped.GetPosition() == pos) conflict = true;

            for (const auto& car : allCars)
                if (car.GetPosition() == pos) conflict = true;

            if (!conflict) valid = true;
            attempts++;
        }

        if (valid)
        {
            allCars.emplace_back(pos);
        }
    }
}

void GameLoop::Render()
{
    if (!gameRunning) return;

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
                    bool drawn = false;

                    for (const auto& pedestrian : allPedestrians)
                    {
                        if (pedestrian.GetPosition() == pos && pedestrian.IsAlive())
                        {
                            std::cout << 'p';
                            drawn = true;
                            break;
                        }
                    }

                    if (!drawn)
                    {
                        for (const auto& car : allCars)
                        {
                            if (car.GetPosition() == pos)
                            {
                                std::cout << 'C';
                                drawn = true;
                                break;
                            }
                        }

                        std::cout << map.GetCell(pos).ToChar();
                    }
                }
            }
            std::cout << "\n";
        }

        std::cout << "Dinero: " << player.GetMoney() << std::endl;
        std::cout << "Vida: " << player.GetHealth() << "\n";
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

    Position pos;
    int attempts = 0;
    bool validPosition = false;

    int islandIndex = 2;
    while (!validPosition && attempts < 100) {
        pos.x = rand() % (map.GetWidth() / 3) + islandIndex * map.GetWidth() / 3 + 1;
        pos.y = rand() % (map.GetHeight() - 2) + 1;

        Cell cell = map.GetCell(pos);
        validPosition = (cell.type != CellType::WALL);

        for (const auto& ped : allPedestrians)
            if (ped.GetPosition() == pos)
                validPosition = false;

        attempts++;
    }

    if (validPosition) {
        finalBoss = new BigSmoke(pos, 300, 50);
        allPedestrians.push_back(*finalBoss);
    }
}