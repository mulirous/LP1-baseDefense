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
    std::shared_ptr<Item> item;
    sf::Clock expiredCounter;
    float expirationTime;
    bool used;

public:
    /// @brief Default constructor
    /// @param item Pointer to item-type object. Must be derived from Item
    /// @param position Position that drop will be rendered
    /// @param expirationSeconds Time that drop will remain on screen until disappears. Default is 7
    Drop(std::shared_ptr<Item> item, sf::Vector2f &position, std::optional<float> expirationSeconds = 7);
    /// @brief Sets drop position
    /// @param position Position's vector
    void setPosition(sf::Vector2f &position);
    /// @brief Gets drop bounds
    /// @return FloatRect with bounds
    const sf::FloatRect getBounds();
    /// @brief Gets sprite of drop's item
    /// @return Pointer to item's sprite
    std::shared_ptr<sf::Sprite> getItemSprite();
    /// @brief Sets drop as used
    /// @note Cannot rollback used state after this
    void markAsUsed();
    /// @brief Get drop state
    /// @return Boolean indicating if drop is still valid
    /// @note A drop can expire because time or if it was used
    bool hasExpired();
    /// @brief Gets drop item
    /// @return Pointer to item
    std::shared_ptr<Item> getItem();
};