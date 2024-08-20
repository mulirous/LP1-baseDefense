#include "../interfaces/ManaPotion.hpp"
#include "../common.h"

ManaPotion::ManaPotion() : Item(BLUE_POTION_IMAGE)
{
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(BLUE_POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}

ManaPotion::ManaPotion(int mana) : Item(BLUE_POTION_IMAGE)
{
    manaAmount = mana;
    idleAnimation = std::make_shared<Animation>(ResourceManager::getTexture(BLUE_POTION_IMAGE), sf::Vector2u(8, 1), 0.08f);
}

int ManaPotion::getMana()
{
    return manaAmount;
};

void ManaPotion::animate(float dt)
{
    idleAnimation->update(dt);
    sprite->setTextureRect(idleAnimation->textureRect);
};