#include "../interfaces/Potion.hpp"
#include "../common.h"

Potion::Potion() : Item(POTION_IMAGE)
{
    healAmount = 10;
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}


Potion::Potion(int healAmount) : Item(POTION_IMAGE)
{
    this->healAmount = healAmount;
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(BLUE_POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}

int Potion::getHealAmount() { return healAmount; }

void Potion::animate(float dt) {
    idleAnimation->update(dt);
    sprite->setTextureRect(idleAnimation->textureRect);
}