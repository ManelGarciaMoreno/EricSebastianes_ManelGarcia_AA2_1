#pragma once
#ifndef CAR_H
#define CAR_H

#include "Position.h"
#include "Direction.h"

class Map;
class Player;
class Car
{
    bool occupied = false;
    Position position;
public:
    Car(const Position& pos);

    void Move(Direction dir, const Map& map);

    bool IsOccupied() const;// { return occupied; }

    void SetOccupied(bool occ);// { occupied = occ; }

    void SetPosition(const Position& pos);// { position = pos; }

    Position GetPosition() const;// { return position; }
private:

};
#endif