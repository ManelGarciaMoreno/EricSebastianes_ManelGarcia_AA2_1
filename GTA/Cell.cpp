#include "Cell.h"

char Cell::ToChar() const 
{
    if (hasPlayer) return 'P';
    if (hasPedestrian) return 'p';

    switch (type) 
    {
    case CellType::WALL: 
        return 'X';
        break;
    case CellType::BRIDGE: 
        return '=';
        break;
    case CellType::MONEY: 
        return '$';
        break;
    default: 
        return ' ';
        break;
    }
}