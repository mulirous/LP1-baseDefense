#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include "Entity.hpp"
#include "RangedWeapon.hpp"
#include <map>
#include "../enums/CharacterDirection.h"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include "Projectile.hpp"
#include "../enums/CharacterAnimation.h"
#include "../src/modules/animation/src/Animation.hpp"

/// @file Character.hpp
/// @class Character
/// @brief Abstract class that represents the characters on the game and defines common behavior for them. Hero and Enemy inherits from this.
/// @details This class is designed to be inherited by Hero and Enemy.
/// It encapsulates common attributes and methods for characters in the game.
/// Also, it handles movement, animations, and basic collisions.
class Character : public Entity
{
protected:
    /// @brief Width of the character in pixels.
    float width;

    /// @brief Height of the character in pixels.
    float height;

    /// @brief Speed of the character in pixels per frame.
    float speed;

    /// @brief The current direction the character is facing.
    CharacterDirection direction;

    /// @brief The current animation state of the character.
    CharacterAnimation animationState;

    /// @brief Map holding available animations for the character.
    /// The key represents the animation name and the value is a shared pointer to the animation object.
    std::shared_ptr<std::map<std::string, std::shared_ptr<Animation>>> animations;

    /// @brief The ranged weapon the character is holding.
    std::shared_ptr<RangedWeapon> weapon;

    /// @brief Pure virtual function to initialize the character's animations.
    /// @note This must be implemented by derived classes to set up the character's animations.
    virtual void initAnimations() = 0;

    /// @brief Pure virtual function to update the character's animation based on the current action and delta time.
    /// @param action The animation action to update.
    /// @param dt Delta time for updating the animation.
    /// @note This must be implemented by derived classes to change character's animations.
    virtual void updateAnimation(const std::string &action, float dt) = 0;

public:
    /// @brief Constructs a Character with the specified attributes.
    /// @param width Character's width in pixels.
    /// @param height Character's height in pixels.
    /// @param speed Character's movement speed in pixels per frame.
    /// @param maxLife Maximum life (HP) of the character.
    /// @param posX Initial X position of the character in the game world.
    /// @param posY Initial Y position of the character in the game world.
    Character(float width, float height, float speed, int maxLife, float posX, float posY);

    /// @brief Constructs a Character with a vector position.
    /// @param width Character's width in pixels.
    /// @param height Character's height in pixels.
    /// @param speed Character's movement speed in pixels per frame.
    /// @param maxLife Maximum life (HP) of the character.
    /// @param position Initial position of the character as a vector.
    Character(float width, float height, float speed, int maxLife, sf::Vector2f position);

    /// @brief Default Character destructor.
    virtual ~Character() = default;

    /// @brief Gets the width of the character.
    /// @return The width of the character in pixels.
    float getWidth() const;

    /// @brief Gets the height of the character.
    /// @return The height of the character in pixels.
    float getHeight() const;

    /// @brief Gets the movement speed of the character.
    /// @return The speed of the character in pixels per frame.
    float getSpeed() const;

    /// @brief Gets the current animation state of the character.
    /// @return The current animation state as a CharacterAnimation enum value.
    CharacterAnimation getAnimationState() const;

    /// @brief Gets the ranged weapon the character is holding.
    /// @return A constant shared pointer to the character's ranged weapon.
    const std::shared_ptr<RangedWeapon> getRangedWeapon();

    /// @brief Sets the current animation state of the character.
    /// @param animation A reference to the animation state to set.
    void setAnimationState(CharacterAnimation &animation);

    /// @brief Checks if the current animation for a specific action has been completed.
    /// @param action The name of the action whose animation state is being checked.
    /// @return True if the animation is completed, false otherwise.
    bool isAnimationCompleted(const std::string &action) const;

    using Entity::isCollidingWith; /// This using is here to make overload visible to derived classes.
    /// @brief Checks if the character is colliding with another character.
    /// @param other A shared pointer to the other character.
    /// @return True if the character's bounding box intersects with the other character's bounding box.
    bool isCollidingWith(std::shared_ptr<Character> other);

    /// @brief Resolves a collision with another character.
    /// @param other A shared pointer to the other character involved in the collision.
    /// @note It adjusts both characters' positions based on the collision overlap.
    void resolveCollision(std::shared_ptr<Character> other);

    /// @brief Takes damage from an attack.
    /// @param damage Amount of damage to be taken.
    /// @note This method is an implementation of `Entity::takeDamage` and allows derived class to implement additional logic by the `virtual` keyword.
    virtual void takeDamage(int damage) override;

    /// @brief Pure virtual function for performing an attack by the character.
    /// @param target The target position to attack towards.
    /// @param dt The time delta for the attack (optional).
    virtual void doAttack(sf::Vector2f &target, float dt = {}) = 0;

    /// @brief Pure virtual function for moving the character.
    /// @param deltaTime The time delta to apply for movement (optional).
    virtual void move(float deltaTime = {}) = 0;
};
