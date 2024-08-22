#pragma once

#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "../common.h"
#include "Entity.hpp"

/// @brief Represents the base which hero must defend and enemies will attack
class Base : public Entity
{
private:
    /// @brief
    float regenerationSeconds;
    sf::Clock regenerationClock;

public:
    /// @brief Default constructor.
    /// @param maxLife Base's max life
    /// @param posX
    /// @param posY
    /// @param texture
    Base(int maxLife, float regenerationSeconds, float posX, float posY);
    virtual ~Base() = default;
    void takeDamage(int damage) override;
    void heal();
};