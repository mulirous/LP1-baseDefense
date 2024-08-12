#pragma once
#include "Item.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <random>
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <optional>

/// @brief Enemy's drop
class Drop
{
private:
    sf::Vector2f position;
    // std::shared_ptr<sf::Sprite> sprite;
    std::shared_ptr<Item> item;
    sf::Clock expiredCounter;
    float expirationTime;
    bool used;

public:
    /// @brief Default constructor
    /// @param item Pointer to item-type object. Must be derived from Item
    /// @param position Position that drop will be rendered
    /// @param expirationSeconds Time that drop will remain on screen until disappears
    Drop(std::shared_ptr<Item> item, sf::Vector2f &position, std::optional<float> expirationSeconds = 7)
    {
        this->item = item;
        this->item->getSprite()->setPosition(position);
        this->position = position;
        expirationTime = *expirationSeconds;
        expiredCounter = sf::Clock();
        expiredCounter.restart();
        used = false;
    }

    void setPosition(sf::Vector2f &position) { this->position = position; }
    const sf::FloatRect getBounds()
    {
        return this->item->getSprite()->getGlobalBounds();
    }
    std::shared_ptr<sf::Sprite> getItemSprite() { return this->item->getSprite(); }
    void setUsed(bool used) { this->used = used; }
    bool hasExpired() { return expiredCounter.getElapsedTime().asSeconds() >= expirationTime || used; }
    std::shared_ptr<Item> getItem() { return item; }
};