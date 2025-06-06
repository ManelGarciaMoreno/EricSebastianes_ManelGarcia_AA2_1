#ifndef MAP_H
#define MAP_H

#include "Cell.h"
#include "Position.h"
#include "GameConfig.h"
#include <vector>

class Map {
public:
    Map(int width, int height);

    void Initialize(const GameConfig& config);
    bool IsPositionValid(const Position& pos) const;
    Cell GetCell(const Position& pos) const;
    void SetCell(const Position& pos, const Cell& cell);

    int GetWidth() const 
    { 
        return width; 
    }

    int GetHeight() const 
    { 
        return height; 
    }

private:
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;

    void CreateRiversAndBridges();
};

#endif