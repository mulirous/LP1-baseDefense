#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../src/modules/texture_manager/src/ResourceManager.hpp"

/// @brief Abstract class to represent an item
class Item
{
protected:
    /// @brief Pointer to item's sprite
    std::shared_ptr<sf::Sprite> sprite;

    /// @brief Default constructor
    /// @param filepath Path to sprite's texture
    /// @note Accessible only by item-derived class
    Item(const std::string &filepath);

public:
    /// @brief Default destructor
    virtual ~Item() = default;
    /// @brief Get item's sprite
    /// @return Pointer to sprite
    std::shared_ptr<sf::Sprite> getSprite();
};