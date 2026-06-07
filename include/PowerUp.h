#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

class PowerUp : public GameObject
{
private:
    sf::RectangleShape shape;

public:
    PowerUp(float x, float y);
    ~PowerUp();

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const;
};

#endif