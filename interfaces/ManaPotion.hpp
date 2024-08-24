#pragma once

#include "Item.hpp"
#include "../src/modules/animation/src/Animation.hpp"

/// @file ManaPotion.hpp
/// @class ManaPotion
/// @brief A class representing a potion that can restore hero's mana.
/// Inherits from the Item class and adds specific functionality for mana potions.
/// @note Mana potions are items that the player can use to restore mana.
class ManaPotion : public Item
{
private:
    /// @brief Amount of mana that this potion restores.
    int manaAmount;

    /// @brief Pointer to the idle animation for the potion.
    std::shared_ptr<Animation> idleAnimation;

public:
    /// @brief Default constructor for the ManaPotion class.
    /// @details Initializes the potion with a default mana restoration of 10 and sets up the idle animation.
    ManaPotion();

    /// @brief Parameterized constructor for the ManaPotion class.
    /// @param mana Amount of mana that the potion provides.
    /// @details Initializes the potion with a specified mana amount and sets up the idle animation.
    ManaPotion(int mana);

    /// @brief Default ManaPotion destructor.
    virtual ~ManaPotion() = default;

    /// @brief Retrieves the amount of mana provided by the potion.
    /// @return The amount of mana that the potion restores.
    int getMana() const;

    /// @brief Update the potion's animation.
    /// @param dt Elapsed time since the last update in seconds.
    /// @note Updates the idle animation based on the elapsed time.
    /// This method is called to ensure that the potion's sprite is animated
    /// correctly while it is in the game world.
    void animate(float dt) override;
};