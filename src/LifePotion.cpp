#include "../interfaces/LifePotion.hpp"
#include "../common.h"

LifePotion::LifePotion() : Item(POTION_IMAGE)
{
    healAmount = 10;
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}

LifePotion::LifePotion(int healAmount) : Item(POTION_IMAGE)
{
    this->healAmount = healAmount;
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}

int LifePotion::getHealAmount() const
{
    return healAmount;
}

void LifePotion::animate(float dt)
{
    idleAnimation->update(dt);
    sprite->setTextureRect(idleAnimation->textureRect);
}
