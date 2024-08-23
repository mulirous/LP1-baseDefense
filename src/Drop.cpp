#include "../interfaces/Drop.hpp"

Drop::Drop(std::shared_ptr<Item> item, sf::Vector2f position, float expirationSeconds)
    : position(position), item(item), expirationTime(expirationSeconds), used(false)
{
    expiredCounter.restart();
}

void Drop::setPosition(sf::Vector2f position)
{
    this->position = position;
}

const sf::FloatRect Drop::getBounds() const
{
    return item->getSprite()->getGlobalBounds();
}

const std::shared_ptr<Item> Drop::getItem() const
{
    return item;
}

const std::shared_ptr<sf::Sprite> Drop::getItemSprite() const
{
    return item->getSprite();
}

void Drop::markAsUsed()
{
    used = true;
}

bool Drop::hasExpired() const
{
    return expiredCounter.getElapsedTime().asSeconds() >= expirationTime || used;
}
