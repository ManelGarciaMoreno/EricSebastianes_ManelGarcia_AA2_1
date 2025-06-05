#include "Map.h"
#include "GameConfig.h"
#include <iostream>

Map::Map(int width, int height) : width(width), height(height) 
{
    grid.resize(height, std::vector<Cell>(width));
}

void Map::Initialize(const GameConfig& config) 
{
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) 
            {
                grid[y][x].type = CellType::WALL;
            }
            else 
            {
                grid[y][x].type = CellType::EMPTY;
            }
        }
    }

    CreateRiversAndBridges();
}

bool Map::IsPositionValid(const Position& pos) 
{
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) 
    {
        return false;
    }
    return grid[pos.y][pos.x].type != CellType::WALL;
}

Cell Map::GetCell(const Position& pos) const 
{
    return grid[pos.y][pos.x];
}

void Map::SetCell(const Position& pos, const Cell& cell) 
{
    grid[pos.y][pos.x] = cell;
}

void Map::CreateRiversAndBridges() 
{
    int islandWidth = width / 3;
    int bridgeOpeningY = height / 2;

    // Between Los Santos and San Fierro
    for (int row = 0; row < height; row++) 
    {
        if (row != bridgeOpeningY) 
        {
            grid[row][islandWidth].type = CellType::WALL;
        }
        else 
        {
            grid[row][islandWidth].type = CellType::BRIDGE;
        }
    }

    // Between San Fierro and Las Venturas
    for (int row = 0; row < height; row++) 
    {
        if (row != bridgeOpeningY) 
        {
            grid[row][islandWidth * 2].type = CellType::WALL;
        }
        else 
        {
            grid[row][islandWidth * 2].type = CellType::BRIDGE;
        }
    }
}