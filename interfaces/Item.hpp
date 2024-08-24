#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../src/modules/texture_manager/src/ResourceManager.hpp"

/// @file Item.hpp
/// @class Item
/// @brief Abstract class to represent an item in the game. Potion, ManaPotion and Quiver inherits from this.
/// @details This class manages a sprite that visually represents the item.
/// Derived classes must implement the `animate` method to define specific animations.
class Item
{
protected:
    /// @brief Shared pointer to the item's sprite.
    std::shared_ptr<sf::Sprite> sprite;

    /// @brief Constructor that initializes the item's sprite with the provided texture.
    /// @param filepath Path to the texture file for the item's sprite.
    /// @note This constructor is protected and can only be accessed by classes derived from `Item`.
    Item(const std::string &filepath);

public:
    /// @brief Default Item destructor.
    virtual ~Item() = default;

    /// @brief Retrieves the sprite associated with this item.
    /// @return A constant shared pointer to the item's sprite.
    const std::shared_ptr<sf::Sprite> getSprite();

    /// @brief Pure virtual function to animate the item.
    /// @param dt Time delta used to advance the animation based on the frame rate.
    /// This function must be overridden by derived classes to provide specific animation behavior.
    virtual void animate(float dt) = 0;
};