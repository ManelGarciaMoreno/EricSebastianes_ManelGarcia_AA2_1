#include "Car.h"

Car::Car(const Position& pos) : position(pos), occupied(false) {}

void Car::MoveUp() { position.y--; }
void Car::MoveDown() { position.y++; }
void Car::MoveLeft() { position.x--; }
void Car::MoveRight() { position.x++; }

Position Car::GetPosition() const { return position; }

void Car::SetPosition(const Position& pos) { position = pos; }

bool Car::IsOccupied() const { return occupied; }
void Car::SetOccupied(bool occ) { occupied = occ; }