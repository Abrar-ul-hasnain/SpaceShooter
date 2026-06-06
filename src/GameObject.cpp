#include "../include/GameObject.h"

GameObject::GameObject(float x, float y)
    : sprite(texture)
{
    position.x = x;
    position.y = y;
    velocity.x = 0;
    velocity.y = 0;
    isActive = true;
}

GameObject::~GameObject()
{
}

sf::FloatRect GameObject::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool GameObject::getIsActive() const
{
    return isActive;
}

void GameObject::setIsActive(bool active)
{
    isActive = active;
}

sf::Vector2f GameObject::getPosition() const
{
    return position;
}

void GameObject::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}