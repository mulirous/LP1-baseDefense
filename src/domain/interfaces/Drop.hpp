#pragma once
#include "../interfaces/Item.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <random>
#include "../modules/texture_manager/src/ResourceManager.hpp"
#include <optional>

/// @brief Enemy's drop
class Drop
{
private:
    sf::Vector2f position;
    std::shared_ptr<sf::Sprite> sprite;
    std::shared_ptr<Item> item;
    sf::Clock expiredCounter;
    float expirationTime;
    bool used;

public:
    Drop(std::shared_ptr<Item> item, sf::Vector2f &position, std::optional<float> expirationSeconds = std::nullopt)
    {
        sprite = std::make_shared<sf::Sprite>();
        sprite->setTexture(*ResourceManager::getTexture(POTION_IMAGE));
        this->item = item;
        this->position = position;
        this->sprite->setPosition(position);
        expirationTime = *expirationSeconds;
        expiredCounter = sf::Clock();
        expiredCounter.restart();
        used = false;
    }

    void setPosition(sf::Vector2f &position) { this->position = position; }
    const sf::FloatRect getBounds()
    {
        return this->sprite->getGlobalBounds();
    }
    const sf::Sprite getSprite() { return *sprite; }
    void setUsed(bool used) { this->used = used; }
    bool hasExpired() { return expiredCounter.getElapsedTime().asSeconds() >= expirationTime || used; }
    std::shared_ptr<Item> getItem() { return item; }
};