#pragma once

#include "Item.hpp"
#include "../src/modules/animation/src/Animation.hpp"

/// @file LifePotion.hpp
/// @class LifePotion
/// @brief Represents a potion item that can heal the player.
/// Inherits from the Item class and adds functionality specific to life potions.
/// @note Life potions are items that the player can use to restore health.
class LifePotion : public Item
{
private:
    /// @brief Amount of health restored by the potion.
    int healAmount;

    /// @brief Pointer to the animation of the potion.
    /// @details This animation is used to visually represent the potion's idle state.
    std::shared_ptr<Animation> idleAnimation;

public:
    /// @brief Default constructor for the `Potion` class.
    /// @details Initializes the potion with a default healing amount of 10 and sets up the idle animation.
    LifePotion();

    /// @brief Parameterized constructor for the `Potion` class.
    /// @param healAmount Amount of health restored by the potion.
    /// @details Initializes the potion with a specified healing amount and sets up the idle animation.
    LifePotion(int healAmount);

    /// @brief Default Potion destructor.
    ~LifePotion() = default;

    /// @brief Retrieves the amount of health restored by the potion.
    /// @return The amount of health restored.
    int getHealAmount() const;

    /// @brief Updates the potion's animation.
    /// @param dt Elapsed time since the last update in seconds.
    /// @note Updates the idle animation based on the elapsed time.
    /// This method is called to ensure that the potion's sprite is animated
    /// correctly while it is in the game world.
    void animate(float dt) override;
};