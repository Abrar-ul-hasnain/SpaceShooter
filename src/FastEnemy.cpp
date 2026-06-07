#include "../include/FastEnemy.h"

FastEnemy::FastEnemy(float x, float y) : Enemy(x, y)
{
    // FastEnemy ki properties
    speed = 200.0f;      // Zyada tez
    health = 15;         // Kam health
    pointValue = 20;     // Zyada points

    zigzagTimer = 0.0f;
    zigzagDirection = 1.0f;

    // Alag color — blue
    shape.setFillColor(sf::Color::Magenta);
    shape.setSize(sf::Vector2f(35.0f, 35.0f));
}

FastEnemy::~FastEnemy()
{
}

void FastEnemy::update(float deltaTime)
{
    // Zigzag movement
    zigzagTimer += deltaTime;
    if (zigzagTimer > 0.5f)
    {
        zigzagDirection *= -1.0f;
        zigzagTimer = 0.0f;
    }

    position.x += zigzagDirection * speed * deltaTime;
    position.y += (speed * 0.5f) * deltaTime;
    shape.setPosition(position);

    if (position.y > 650)
        isActive = false;
}