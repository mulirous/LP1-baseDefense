#pragma once

#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "../common.h"
#include "Entity.hpp"

/// @file Base.hpp
/// @class Base
/// @brief Represents the base that the hero must defend and the enemies will attack.
/// @details The Base class is derived from the Entity class and represents a defensive structure
/// in the game. It has regeneration capabilities and can take damage from enemies.
/// The base automatically heals itself after a set period of time, and the hero must protect it from destruction.
class Base : public Entity
{
private:
    /// @brief The amount of time, in seconds, between each regeneration tick.
    float regenerationSeconds;

    /// @brief Clock to track the time elapsed for regeneration.
    sf::Clock regenerationClock;

public:
    /// @brief Constructs a Base with a specific amount of life and regeneration time.
    /// @param maxLife The maximum life of the base.
    /// @param regenerationSeconds The number of seconds between each life regeneration tick.
    Base(int maxLife, float regenerationSeconds);

    /// @brief Default destructor for the Base class.
    virtual ~Base() = default;

    /// @brief Reduces the life of the base by a specified damage amount.
    /// @param damage The amount of damage to apply to the base.
    /// @details If the damage is less than or equal to 0, no damage is applied.
    void takeDamage(int damage) override;

    /// @brief Heals the base by a fixed amount after a certain time interval.
    /// @details The base will regenerate life every `regenerationSeconds` seconds, up to its maximum life.
    void heal();
};
