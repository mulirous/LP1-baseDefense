#pragma once

#include "Item.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

/// @file Drop.hpp
/// @class Drop
/// @brief Represents a drop item that appears in the game world, such as loot from fallen enemies.
/// @details This class manages the position, expiration, and state of the drop.
/// It holds a reference to an `Item` and determines whether the drop is still valid or has expired.
class Drop
{
private:
    /// @brief Position of the drop in the game world.
    sf::Vector2f position;

    /// @brief The item that the drop represents.
    std::shared_ptr<Item> item;

    /// @brief Clock to track the time since the drop was created.
    sf::Clock expiredCounter;

    /// @brief Time in seconds that the drop will remain on screen before disappearing.
    float expirationTime;

    /// @brief Boolean indicating whether the drop has been used.
    bool used;

public:
    /// @brief Constructs a Drop with specified attributes.
    /// @param item A shared pointer to the item associated with the drop. Must be derived from `Item`.
    /// @param position The position where the drop will be rendered in the game world.
    /// @param expirationSeconds Time in seconds that the drop will remain on screen before disappearing. Default is 7 seconds.
    Drop(std::shared_ptr<Item> item, sf::Vector2f position, float expirationSeconds = 7);

    /// @brief Sets the position of the drop.
    /// @param position The new position of the drop as a vector.
    void setPosition(sf::Vector2f position);

    /// @brief Gets the bounding box of the drop.
    /// @return An `sf::FloatRect` representing the bounds of the drop.
    const sf::FloatRect getBounds() const;

    /// @brief Gets the item associated with the drop.
    /// @return A constant shared pointer to the item.
    const std::shared_ptr<Item> getItem() const;

    /// @brief Gets the sprite of the item's drop.
    /// @return A constant shared pointer to the sprite of the item.
    const std::shared_ptr<sf::Sprite> getItemSprite() const;

    /// @brief Marks the drop as used.
    /// @note Once marked as used, the state cannot be reverted.
    void markAsUsed();

    /// @brief Checks if the drop has expired.
    /// @return True if the drop has expired due to time or if it has been used or false otherwise.
    bool hasExpired() const;
};