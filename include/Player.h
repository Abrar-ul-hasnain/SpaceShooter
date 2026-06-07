#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
private:
    float speed;
    int health;
    int maxHealth;
    float fireRate;
    float fireCooldown;
    sf::RectangleShape shape;

public:
    Player(float x, float y);
    ~Player();

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    void handleInput(float deltaTime);
    bool canShoot();
    void takeDamage(int damage);

    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;
    sf::FloatRect getBounds() const;
    void setHealth(int h);
};

#endif