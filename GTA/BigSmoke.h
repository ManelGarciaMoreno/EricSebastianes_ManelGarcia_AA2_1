#ifndef BIGSMOKE_H
#define BIGSMOKE_H

#include "Pedestrian.h"

class BigSmoke : public Pedestrian {
public:
    BigSmoke(Position pos, int health, int attackPower);

    bool IsBigSmoke() const override { return true; }

    bool CanBeRunOver() const override { return false; }
};

#endif
