#include "../interfaces/Base.hpp"
#include <cmath>
#include <iostream>

Base::Base(int maxLife, float regenerationSeconds, float posX, float posY)
    : Entity(maxLife, {posX, posY}), regenerationSeconds(regenerationSeconds)
{
    regenerationClock = sf::Clock();
    regenerationClock.restart();
    sprite->setTexture(*ResourceManager::getTexture(BASE_IMAGE));
    sprite->setScale(0.3, 0.3);
    sprite->setPosition(posX - (sprite->getGlobalBounds().width / 2),
                        posY - (sprite->getGlobalBounds().height / 2));
}

void Base::takeDamage(int damage)
{
    if (damage <= 0)
        return;
    currentLife -= damage;
}

void Base::heal()
{
    if (regenerationClock.getElapsedTime().asSeconds() >= regenerationSeconds)
    {
        if (currentLife < maximumLife)
        {
            currentLife += 50;
            if (currentLife > maximumLife)
            {
                currentLife = maximumLife;
            }
        }
    }
}