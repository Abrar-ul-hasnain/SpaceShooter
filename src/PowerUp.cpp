#include "../include/PowerUp.h"

PowerUp::PowerUp(float x, float y) : GameObject(x, y)
{
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
}

PowerUp::~PowerUp()
{
}

void PowerUp::update(float deltaTime)
{
    position.y += 80.0f * deltaTime;
    shape.setPosition(position);

    if (position.y > 650)
        isActive = false;
}

void PowerUp::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect PowerUp::getBounds() const
{
    return shape.getGlobalBounds();
}