#include "../interfaces/Base.hpp"
#include <iostream>

Base::Base(int maxLife, float posX, float posY)
{
    this->maxLife = maxLife;
    this->currentLife = maxLife;
    this->currentPosition = {posX, posY};

    baseSprite = std::make_shared<sf::Sprite>();
    baseSprite->setTexture(*ResourceManager::getTexture(BASE_IMAGE));
    baseSprite->setScale(0.6, 0.6);
    baseSprite->setPosition(posX - (baseSprite->getGlobalBounds().width / 2),
                            posY - (baseSprite->getGlobalBounds().height / 2));
}

std::shared_ptr<sf::Sprite> Base::getSprite()
{
    return this->baseSprite;
}

sf::Vector2f Base::getPosition()
{
    return this->currentPosition;
}

void Base::takeDamage(int damage)
{
    if (damage <= 0)
        return;
    this->currentLife -= damage;
}

int Base::getLife()
{
    return this->currentLife;
}

bool Base::isCollidingWith(const sf::FloatRect &other)
{
    return baseSprite->getGlobalBounds().intersects(other);
}