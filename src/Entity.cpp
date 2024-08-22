#include "../interfaces/Entity.hpp"

Entity::Entity(int life)
{
    maximumLife = life;
    currentLife = life;
    sprite = std::make_shared<sf::Sprite>();
}

Entity::Entity(int life, sf::Vector2f pos) : Entity(life)
{
    currentPosition = pos;
};

Entity::Entity(int life, float posX, float posY) : Entity(life, {posX, posY}) {};

int Entity::getMaxLife()
{
    return maximumLife;
}

int Entity::getLife()
{
    return currentLife;
}

sf::Vector2f Entity::getCurrentPosition()
{
    return currentPosition;
}

sf::FloatRect Entity::getBounds()
{
    return sprite->getGlobalBounds();
}

void Entity::setCurrentPosition(sf::Vector2f &position)
{
    currentPosition = position;
}

bool Entity::isCollidingWith(const sf::FloatRect &rect)
{
    return getBounds().intersects(rect);
}

std::shared_ptr<sf::Sprite> Entity::getSprite()
{
    return sprite;
};