#include "../interfaces/Base.hpp"
#include <cmath>
#include <iostream>

Base::Base(int maxLife, float regenerationSeconds, float posX, float posY)
    : maxLife(maxLife), currentLife(maxLife), regenerationSeconds(regenerationSeconds), currentPosition({posX, posY})
{
    regenerationClock = sf::Clock();
    regenerationClock.restart();
    baseSprite = std::make_shared<sf::Sprite>();
    baseSprite->setTexture(*ResourceManager::getTexture(BASE_IMAGE));
    baseSprite->setScale(0.3, 0.3);
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
    currentLife -= damage;
}

int Base::getLife()
{
    return currentLife;
}

void Base::heal()
{
    if (regenerationClock.getElapsedTime().asSeconds() >= regenerationSeconds)
    {
        if (currentLife < maxLife)
        {
            currentLife += 50;
            if (currentLife > maxLife)
            {
                currentLife = maxLife;
            }
        }
    }
}

bool Base::isCollidingWith(const sf::FloatRect &other)
{
    return baseSprite->getGlobalBounds().intersects(other);
}