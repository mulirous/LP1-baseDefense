#pragma once

#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Quiver.hpp"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <memory>
#include "Item.hpp"
#include "ManaPotion.hpp"
#include "RangedWeapon.hpp"
#include "LifePotion.hpp"
#include <type_traits>

/// @file Hero.hpp
/// @class Hero
/// @brief Represents the main character in the game known as "Hero". Derived from Character class.
/// @details The Hero uses a ranged weapon and has animations for idle, walking, and attacking.
/// It overrides some important methods from Character to implement its own behavior,
/// such as handling movements, attacks and healing.
class Hero : public Character
{
protected:
    /// @brief The target position that the hero will move towards
    sf::Vector2f targetPosition;

    /// @brief Heals the hero by a specified amount
    /// @param healAmount The amount of health to be restored to the hero
    /// @note If the amount of health restored would result in the hero's health
    /// exceeding the maximum life points, the hero's health is capped at the maximum.
    void heal(int healAmount);

    /// @brief Recharges the hero's ammunition or mana by a specified amount
    /// @param ammo The amount of ammunition or mana to recharge
    void recharge(int ammo);

    /// @brief Updates the hero's animation based on the current action and time
    /// @param action A string representing the action to be animated (e.g., "idle", "walk", "attack")
    /// @param dt The time elapsed since the last frame, used to advance the animation
    /// @note This method changes the hero's animation and updates the texture and sprite
    /// based on the current action and direction.
    void updateAnimation(const std::string &action, float dt) override;

public:
    /// @brief Constructs a hero with the given size, speed, max life, weapon damage, and initial position.
    /// @param width The width of the hero.
    /// @param height The height of the hero.
    /// @param speed The movement speed of the hero.
    /// @param maxLife The maximum life of the hero.
    /// @param posX The initial X position of the hero.
    /// @param posY The initial Y position of the hero.
    Hero(float width, float height, float speed, int maxLife, float posX, float posY);

    /// @brief Default Hero destructor.
    virtual ~Hero() = default;

    /// @brief Sets the target position that the hero will move towards.
    /// @param target The new target position for the hero.
    /// @note If the target position isn't within the window's bounds, target is not set.
    void setTargetPosition(sf::Vector2f target);

    /// @brief Initializes the hero's animations for different actions.
    /// @note This method loads the textures and sets up the frames for each animation.
    /// It is called during the construction of the hero.
    void initAnimations() override;

    /// @brief Moves the hero towards the target position based on delta time.
    /// @param deltaTime The time elapsed since the last frame.
    /// @note This method calculates the direction and movement needed to move the hero
    /// towards the target position. It also updates the hero's animation state
    /// between idle, walking, and attacking based on the hero's actions.
    void move(float deltaTime) override;

    /// @brief Executes an attack action in the direction of a target
    /// @param target The position of the target to attack
    /// @param dt Optional delta time for animation updates
    /// @note This method triggers the hero's attack animation and calculates the direction
    /// of the attack based on the position of the target. The attack is performed by
    /// the hero's weapon if it is ready.
    void doAttack(sf::Vector2f &target, float dt = {}) override;

    /// @brief Uses an item such as a potion, mana potion, or quiver to heal the hero or recharge the hero's weapon.
    /// @tparam T The type of the item being used, which must derive from the Item class.
    /// @param item A shared pointer to the item to be used.
    /// @note This method checks the type of the item passed to it, such as a potion or quiver,
    /// and performs the appropriate action (healing, recharging mana, or replenishing ammo, the last one's deprecated).
    /// If the item is not of a valid type, the method does nothing.
    template <typename T>
    void useItem(std::shared_ptr<T> item)
    {
        static_assert(std::is_base_of<Item, T>::value, "Not an item.");
        try
        {
            auto lifePotion = std::dynamic_pointer_cast<LifePotion>(item);
            if (lifePotion)
            {
                int life = lifePotion->getHealAmount();
                heal(life);
                return;
            }
            auto manaPotion = std::dynamic_pointer_cast<ManaPotion>(item);
            if (manaPotion)
            {
                int mana = manaPotion->getMana();
                recharge(mana);
                return;
            }
            auto quiver = std::dynamic_pointer_cast<Quiver>(item);
            if (quiver)
            {
                int ammo = quiver->getArrows();
                recharge(ammo);
                return;
            }
        }
        catch (std::exception &ex)
        {
            return;
        }
    }
};