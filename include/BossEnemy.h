#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "Enemy.h"

class BossEnemy : public Enemy
{
private:
    float moveDirection;
    float shootTimer;
    float shootRate;
    int phase;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBg;

public:
    BossEnemy(float x, float y);
    ~BossEnemy();

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    bool isDefeated() const;
};

#endif