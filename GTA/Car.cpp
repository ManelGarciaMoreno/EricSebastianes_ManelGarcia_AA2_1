#include "Car.h"
#include "Player.h"

Car::Car(const Position& pos) : position(pos), occupied(false) {}

void Car::Move(Player::Direction dir, const Map& map)
{
    Position newPosition = position;

    switch (dir)
    {
    case Player::Direction::UP:
        newPosition.y -= 1;
        break;
    case Player::Direction::DOWN:
        newPosition.y += 1;
        break;
    case Player::Direction::LEFT:
        newPosition.x -= 1;
        break;
    case Player::Direction::RIGHT:
        newPosition.x += 1;
        break;
    }

    if (map.IsPositionValid(newPosition))
    {
        position = newPosition;
    }
}


Position Car::GetPosition() const { return position; }

void Car::SetPosition(const Position& pos) { position = pos; }

bool Car::IsOccupied() const { return occupied; }
void Car::SetOccupied(bool occ) { occupied = occ; }