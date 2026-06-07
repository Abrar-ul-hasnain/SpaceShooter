#ifndef FASTENEMY_H
#define FASTENEMY_H

#include "Enemy.h"

class FastEnemy : public Enemy
{
private:
    float zigzagTimer;
    float zigzagDirection;

public:
    FastEnemy(float x, float y);
    ~FastEnemy();

    void update(float deltaTime) override;
};

#endif