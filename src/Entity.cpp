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

int Entity::getMaxLife() const
{
    return maximumLife;
}

int Entity::getLife() const
{
    return currentLife;
}

const sf::Vector2f &Entity::getCurrentPosition()
{
    return currentPosition;
}

sf::FloatRect Entity::getBounds()
{
    if (!sprite)
        return sf::FloatRect(0, 0, 0, 0);

    return sprite->getGlobalBounds();
}

void Entity::setCurrentPosition(sf::Vector2f position)
{
    currentPosition = position;
}

bool Entity::isCollidingWith(const sf::FloatRect &rect)
{
    return getBounds().intersects(rect);
}

const std::shared_ptr<sf::Sprite> Entity::getSprite()
{
    return sprite;
};