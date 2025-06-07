#pragma once
#ifndef CAR_H
#define CAR_H

#include "Position.h"
#include "Player.h"

class Map;
class Player;

class Car
{
    bool occupied = false;
    Position position;
public:
    Car(const Position& pos);

   /* void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();*/

    void Move(Player::Direction dir, const Map& map);

    bool IsOccupied() const;// { return occupied; }

    void SetOccupied(bool occ);// { occupied = occ; }

    void SetPosition(const Position& pos);// { position = pos; }

    Position GetPosition() const;// { return position; }
private:

};
#endif