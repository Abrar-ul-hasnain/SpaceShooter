#include "../include/Enemy.h"

Enemy::Enemy(float x, float y) : GameObject(x, y)
{
    speed = 100.0f;
    health = 30;
    pointValue = 10;

    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);
}

Enemy::~Enemy()
{
}

void Enemy::update(float deltaTime)
{
    position.y += speed * deltaTime;
    shape.setPosition(position);

    if (position.y > 650)
        isActive = false;
}

void Enemy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const
{
    return shape.getGlobalBounds();
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
        isActive = false;
}

bool Enemy::isDead() const
{
    return health <= 0;
}

int Enemy::getPointValue() const
{
    return pointValue;
}