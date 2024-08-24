#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "RangedWeapon.hpp"
#include "../enums/EnemyState.h"
#include "Character.hpp"
#include "Drop.hpp"
#include <random>
#include <type_traits>
#include <memory>

/// @file Enemy.hpp
/// @class Enemy
/// @brief Represents an enemy character in the game. Derived from Character class.
/// @details It overrides some important methods from Character to implement its own behavior, such as handling movements and attacks,
/// and extras, such as drops when dies.
class Enemy : public Character
{
protected:
    /// @brief The target position the enemy is moving toward.
    sf::Vector2f targetPosition;

    /// @brief The current state of the enemy (ALIVE, DEAD, etc.).
    /// @note This flag is important for managing the enemy's resources after death (like drops).
    EnemyState state;

    /// @brief Timer to track how long it has been since the enemy died.
    sf::Clock clockDeath;

    /// @brief Determines if the enemy has a drop after its death.
    bool drop;

    /// @brief Internal method to handle the enemy's death process.
    /// Sets the enemy's state to DEAD and restarts the death timer to count after this moment.
    void kill();

    /// @brief Updates the enemy's animation based on the action being performed.
    /// @param action The current action (e.g., "walk", "attack").
    /// @param dt The delta time between frames.
    /// @note This method changes the enemy's animation and updates the texture and sprite
    /// based on the current action and direction.
    void updateAnimation(const std::string &action, float dt) override;

public:
    /// @brief Constructs an enemy with the given size, speed, max life, weapon damage, and initial position.
    /// @param width The width of the enemy.
    /// @param height The height of the enemy.
    /// @param speed The movement speed of the enemy.
    /// @param maxLife The maximum life of the enemy.
    /// @param weaponDamage The damage dealt by the enemy's weapon.
    /// @param x The initial X position of the enemy.
    /// @param y The initial Y position of the enemy.
    /// @param cX The target X position.
    /// @param cY The target Y position.
    Enemy(float width, float height, float speed, int maxLife, int weaponDamage, float x, float y, float cX, float cY);

    /// @brief Constructs an enemy with the given size, speed, max life, weapon damage, and initial position.
    /// Uses an sf::Vector2f for the initial and target positions.
    Enemy(float width, float height, float speed, int maxLife, int weaponDamage, sf::Vector2f position, float cX, float cY);

    /// @brief Constructs an enemy with the given size, speed, max life, weapon damage, and initial position.
    /// Uses sf::Vector2f for both position and target.
    Enemy(float width, float height, float speed, int maxLife, int weaponDamage, sf::Vector2f position, sf::Vector2f target);

    /// @brief Default Enemy destructor.
    virtual ~Enemy() = default;

    /// @brief Checks if the enemy is dead.
    /// @return True if the enemy's state is DEAD, false otherwise.
    bool isDead();

    /// @brief Gets the time elapsed since the enemy's death.
    /// @return The time in seconds since death.
    float getTimeSinceDeath();

    /// @brief Checks if the enemy has a drop after death.
    /// @return True if the enemy drops an item, false otherwise.
    bool hasDrop();

    /// @brief Applies damage to the enemy and checks if it should die.
    /// @param damage The amount of damage dealt to the enemy.
    void takeDamage(int damage) override;

    /// @brief Initializes the enemy's animations for different actions.
    /// @note This method loads the textures and sets up the frames for each animation.
    /// It is called during the construction of the enemy.
    void initAnimations() override;

    /// @brief Moves the enemy towards its target position.
    /// @param deltaTime The time elapsed since the last frame.
    /// @note This method calculates the direction and movement needed to move the enemy
    /// towards the target position. It also updates the enemy's animation state
    /// between walking and attacking based on the enemy's actions.
    void move(float deltaTime) override;

    /// @brief Executes the enemy's attack action.
    /// @param target The position where the enemy is attacking.
    /// @param dt The delta time between frames.
    /// @note This method triggers the enemy's attack animation and calculates the direction
    /// of the attack based on the position of the target. The attack is performed by
    /// the enemy's weapon if it is ready.
    void doAttack(sf::Vector2f &target, float dt = {}) override;
};