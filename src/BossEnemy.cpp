#include "../include/BossEnemy.h"

BossEnemy::BossEnemy(float x, float y) : Enemy(x, y)
{
    health = 200;
    pointValue = 500;
    speed = 80.0f;
    moveDirection = 1.0f;
    shootTimer = 0.0f;
    shootRate = 2.0f;
    phase = 1;

    // Boss bara hoga
    shape.setSize(sf::Vector2f(100.0f, 80.0f));
    shape.setFillColor(sf::Color(255, 100, 0)); // Orange
    shape.setPosition(position);

    // Health bar background
    healthBarBg.setSize(sf::Vector2f(200.0f, 15.0f));
    healthBarBg.setFillColor(sf::Color::Red);
    healthBarBg.setPosition({300.0f, 20.0f});

    // Health bar
    healthBar.setSize(sf::Vector2f(200.0f, 15.0f));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition({300.0f, 20.0f});
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::update(float deltaTime)
{
    // Phase 2 - health 100 se kam ho to tez ho jao
    if (health <= 100 && phase == 1)
    {
        phase = 2;
        speed = 150.0f;
        shape.setFillColor(sf::Color::Red);
    }

    // Left right move karo
    position.x += moveDirection * speed * deltaTime;
    shape.setPosition(position);

    // Wall se bounce karo
    if (position.x > 700.0f)
        moveDirection = -1.0f;
    if (position.x < 0.0f)
        moveDirection = 1.0f;

    // Health bar update
    float healthPercent = health / 200.0f;
    healthBar.setSize(sf::Vector2f(200.0f * healthPercent, 15.0f));

    if (health <= 0)
        isActive = false;
}

void BossEnemy::draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(healthBarBg);
    window.draw(healthBar);
}

bool BossEnemy::isDefeated() const
{
    return health <= 0;
}