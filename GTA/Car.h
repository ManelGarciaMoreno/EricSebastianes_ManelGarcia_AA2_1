#pragma onceAdd commentMore actions
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

    void SetOccupied(bool occ);// { occupied = occ; }Add commentMore actions

    void SetPosition(const Position& pos);// { position = pos; }

    Position GetPosition() const;// { return position; }
private:

};
#endif