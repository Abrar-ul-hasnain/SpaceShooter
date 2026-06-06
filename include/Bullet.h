#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"

class Bullet : public GameObject
{
private:
    float speed;
    int damage;
    sf::RectangleShape shape;

public:
    Bullet(float x, float y);
    ~Bullet();

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const;

    int getDamage() const;
};

#endif