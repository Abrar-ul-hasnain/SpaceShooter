#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

class GameObject
{
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool isActive;

public:
    // Constructor
    GameObject(float x, float y);

    // Virtual Destructor
    virtual ~GameObject();

    // Pure Virtual Functions
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Common Functions
    sf::FloatRect getBounds() const;
    bool getIsActive() const;
    void setIsActive(bool active);
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
};

#endif