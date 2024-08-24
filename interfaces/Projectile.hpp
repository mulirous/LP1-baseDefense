#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include "../common.h"
#include <map>
#include <list>
#include "../src/modules/animation/src/Animation.hpp"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"

/// @file Projectile.hpp
/// @class Projectile
/// @brief Represents a projectile in the game, which moves towards a target and deals damage.
/// It handles the projectile's rendering, animation and movement logic.
class Projectile
{
private:
    /// @brief Current position of the projectile in game world.
    sf::Vector2f position;

    /// @brief Target position that the projectile is moving towards.
    sf::Vector2f target;

    /// @brief Velocity of the projectile.
    float velocity;

    /// @brief Amount of damage the projectile will inflict upon impact.
    int damage;

    /// @brief Shared pointer to the sprite representing the projectile.
    std::shared_ptr<sf::Sprite> sprite;

    /// @brief Map holding available animations for the projectile.
    /// The key represents the animation name and the value is a shared pointer to the animation object.
    std::shared_ptr<std::map<std::string, std::shared_ptr<Animation>>> animations;

    /// @brief Initializes the projectile's animations.
    /// @details This method sets up animations specific to the projectile based on its type (e.g., hero or enemy).
    void initAnimations();

public:
    /// @brief Constructs a Projectile with the given attributes.
    /// @param damage Amount of damage the projectile will inflict.
    /// @param velocity Speed at which the projectile moves.
    /// @param position Initial position of the projectile.
    /// @param target Target position the projectile is moving towards.
    /// @param isHero Flag indicating whether the projectile is fired by a hero or an enemy.
    /// @details Depending on whether the projectile is fired by a hero or an enemy,
    /// it initializes the sprite and animations accordingly.
    /// For hero projectiles, an animation is initialized; for enemy projectiles, the sprite is set with a rotation.
    Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target, bool isHero);

    /// @brief Retrieves the current sprite of the projectile.
    /// @return A constant reference to the sprite.
    const sf::Sprite &getSprite();

    /// @brief Gets the bounding rectangle of the projectile's sprite. Used to detect collisions.
    /// @return A FloatRect representing the bounding box of the sprite.
    sf::FloatRect getBounds();

    /// @brief Retrieves the amount of damage the projectile will inflict.
    /// @return The damage amount.
    int getDamage() const;

    /// @brief Updates the projectile's state.
    /// @param deltaTime Time elapsed since the last update.
    /// @details Moves the projectile towards its target, updates the animation if present and sets the sprite's position.
    /// The movement is calculated based on the projectile's velocity and direction towards the target.
    void update(float deltaTime);

    /// @brief Checks if the projectile has moved off the screen.
    /// @return True if the projectile is off the screen or false otherwise.
    /// @details A projectile is considered off the screen if its position is outside the visible game window's dimensions.
    bool isOffScreen() const;

    /// @brief Checks if the projectile has reached its target.
    /// @return True if the projectile is close enough to the target or false otherwise.
    /// @details The conclusion about if the projectile has reached its target is based on its proximity
    /// to the target position.
    bool hasReachedTarget() const;
};