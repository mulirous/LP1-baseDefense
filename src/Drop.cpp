#include "../interfaces/Drop.hpp"
Drop::Drop(std::shared_ptr<Item> item, sf::Vector2f &position, std::optional<float> expirationSeconds)
{
    this->item = item;
    this->item->getSprite()->setPosition(position);
    this->position = position;
    expirationTime = *expirationSeconds;
    expiredCounter = sf::Clock();
    expiredCounter.restart();
    used = false;
}

void Drop::setPosition(sf::Vector2f &position)
{
    this->position = position;
}

const sf::FloatRect Drop::getBounds()
{
    return this->item->getSprite()->getGlobalBounds();
}

std::shared_ptr<sf::Sprite> Drop::getItemSprite()
{
    return this->item->getSprite();
}

void Drop::markAsUsed()
{
    used = true;
}

bool Drop::hasExpired()
{
    return expiredCounter.getElapsedTime().asSeconds() >= expirationTime || used;
}

std::shared_ptr<Item> Drop::getItem()
{
    return item;
}