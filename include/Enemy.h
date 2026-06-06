#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject
{
protected:
    float speed;
    int health;
    int pointValue;
    sf::RectangleShape shape;

public:
    Enemy(float x, float y);
    ~Enemy();

    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const;

    void takeDamage(int damage);
    bool isDead() const;
    int getPointValue() const;
};

#endif