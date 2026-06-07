#include "../include/Player.h"

Player::Player(float x, float y) : GameObject(x, y)
{
    speed = 200.0f;
    health = 100;
    maxHealth = 100;
    fireRate = 0.3f;
    fireCooldown = 0.0f;

    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
}

Player::~Player()
{
}

void Player::handleInput(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        position.x -= speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        position.x += speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        position.y -= speed * deltaTime;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        position.y += speed * deltaTime;

    if (position.x < 0) position.x = 0;
    if (position.x > 750) position.x = 750;
    if (position.y < 0) position.y = 0;
    if (position.y > 550) position.y = 550;

    shape.setPosition(position);
}

void Player::update(float deltaTime)
{
    handleInput(deltaTime);

    if (fireCooldown > 0)
        fireCooldown -= deltaTime;
}
void Player::setHealth(int h)
{
    health = h;
    if (health > maxHealth) health = maxHealth;
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Player::canShoot()
{
    if (fireCooldown <= 0)
    {
        fireCooldown = fireRate;
        return true;
    }
    return false;
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

int Player::getHealth() const
{
    return health;
}

int Player::getMaxHealth() const
{
    return maxHealth;
}

bool Player::isDead() const
{
    return health <= 0;
}
sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}