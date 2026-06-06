#include "../include/Bullet.h"

Bullet::Bullet(float x, float y) : GameObject(x, y)
{
    speed = 400.0f;
    damage = 10;

    shape.setSize(sf::Vector2f(5.0f, 15.0f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
}

Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{
    position.y -= speed * deltaTime;
    shape.setPosition(position);

    if (position.y < 0)
        isActive = false;
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const
{
    return shape.getGlobalBounds();
}

int Bullet::getDamage() const
{
    return damage;
}