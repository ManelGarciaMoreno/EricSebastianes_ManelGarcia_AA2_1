#ifndef CELL_H
#define CELL_H

#include "Position.h"

enum class CellType 
{
    EMPTY,
    WALL,
    BRIDGE,
    MONEY,
    INVALID
};

struct Cell 
{
    CellType type = CellType::EMPTY;
    int moneyInCell = 0;
    bool hasPlayer = false;
    bool hasPedestrian = false;

    char ToChar() const;
};

#endif