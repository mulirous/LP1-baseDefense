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

/// @class Character
/// @brief An abstract base class representing a game character, designed to be inherited by Hero and Enemy.
///
/// This class encapsulates common attributes and methods for characters in the game.
/// It handles movement, animations, and basic collisions.
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

    std::shared_ptr<RangedWeapon> weapon;

    /// @brief Pure virtual function to initialize the character's animations.
    /// This must be implemented by derived classes.
    virtual void initAnimations() = 0;
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

    /// @brief Default virtual destructor.
    virtual ~Character() = default;

    /// @brief Gets the width of the character.
    /// @return The width of the character in pixels.
    float getWidth();

    /// @brief Gets the height of the character.
    /// @return The height of the character in pixels.
    float getHeigth();

    /// @brief Gets the movement speed of the character.
    /// @return The speed of the character in pixels per frame.
    float getSpeed();

    /// @brief Gets the current animation state of the character.
    /// @return The current animation state as a `CharacterAnimation` enum value.
    CharacterAnimation getAnimationState() const;

    std::shared_ptr<RangedWeapon> getRangedWeapon();

    /// @brief Sets the current animation state of the character.
    /// @param animation A reference to the animation state to set.
    void setAnimationState(CharacterAnimation &animation);

    /// @brief Checks if the current animation for a specific action has been completed.
    /// @param action The name of the action whose animation state is being checked.
    /// @return True if the animation is completed, false otherwise.
    bool isAnimationCompleted(const std::string &action);

    /// @brief Checks if the character is colliding with another character.
    /// @param other A shared pointer to the other character.
    /// @return True if the character's bounding box intersects with the other character's bounding box.
    bool isCollidingWith(std::shared_ptr<Character> other);

    using Entity::isCollidingWith; //< Need this using to turn possible derived classes access them.

    /// @brief Resolves a collision with another character.
    /// This method adjusts both characters' positions based on the collision overlap.
    /// @param other A shared pointer to the other character involved in the collision.
    void resolveCollision(std::shared_ptr<Character> other);

    /// @brief Pure virtual function for handling damage taken by the character.
    /// @param damage The amount of damage to apply to the character's current life.
    virtual void takeDamage(int damage) = 0;

    /// @brief Pure virtual function for performing an attack by the character.
    /// @param target The target position to attack towards.
    /// @param dt The time delta for the attack (optional).
    virtual void doAttack(sf::Vector2f &target, float dt = {}) = 0;

    /// @brief Pure virtual function for moving the character.
    /// @param deltaTime The time delta to apply for movement (optional).
    virtual void move(float deltaTime = {}) = 0;
};
