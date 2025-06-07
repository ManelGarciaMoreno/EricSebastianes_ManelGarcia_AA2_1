#pragma once
#ifndef CAR_H
#define CAR_H

#include "Position.h"

class Car
{
    bool occupied = false;
    Position position;
public:
    Car(const Position& pos);

    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    bool IsOccupied() const;// { return occupied; }

    void SetOccupied(bool occ);// { occupied = occ; }

    void SetPosition(const Position& pos);// { position = pos; }

    Position GetPosition() const;// { return position; }
private:

};
#endif