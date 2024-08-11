#include "interfaces/Item.hpp"

Item::Item(const std::string &filepath)
{
    sprite = std::make_shared<sf::Sprite>();
    sprite->setTexture(*ResourceManager::getTexture(filepath));
}

std::shared_ptr<sf::Sprite> Item::getSprite()
{
    return sprite;
}