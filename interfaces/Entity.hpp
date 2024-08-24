#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

/// @file Entity.hpp
/// @class Entity
/// @brief Abstract class that defines common behavior for some game entites. Character and Base inherits from this.
/// @details This class is designed to be inherited by other classes such as Character and Base.
/// It provides common properties like position, life, and collision detection, as well as an interface for handling damage.
class Entity
{
protected:
    /// @brief The current position of the entity in the game world.
    sf::Vector2f currentPosition;

    /// @brief Shared pointer to the sprite representing the visual appearance of the entity.
    std::shared_ptr<sf::Sprite> sprite;

    /// @brief The maximum amount of life the entity can have.
    int maximumLife;

    /// @brief The current amount of life the entity has.
    int currentLife;

public:
    /// @brief Constructs an entity with the given amount of life.
    /// @param life The initial life value (and the maximum).
    Entity(int life);

    /// @brief Constructs an entity with the given amount of life and position.
    /// @param life The initial life value.
    /// @param posX The X coordinate of the initial position.
    /// @param posY The Y coordinate of the initial position.
    Entity(int life, float posX, float posY);

    /// @brief Constructs an entity with the given amount of life and position.
    /// @param life The initial life value.
    /// @param pos The initial position as a 2D vector.
    Entity(int life, sf::Vector2f pos);

    /// @brief Virtual destructor to allow proper cleanup of derived classes.
    virtual ~Entity() = default;

    /// @brief Retrieves the maximum life value of the entity.
    /// @return The maximum life value as an integer.
    int getMaxLife() const;

    /// @brief Retrieves the current life value of the entity.
    /// @return The current life value as an integer.
    int getLife() const;

    /// @brief Retrieves the current sprite of the entity.
    /// @return A constant shared pointer to the sprite representing the entity.
    const std::shared_ptr<sf::Sprite> getSprite();

    /// @brief Gets the global bounding box of the entity's sprite. Used to detect collisions.
    /// @return A FloatRect representing the bounding box of the sprite.
    sf::FloatRect getBounds();

    /// @brief Gets the current position of the entity in the game world.
    /// @return A constant reference to a Vector2f representing the current position.
    const sf::Vector2f &getCurrentPosition();

    /// @brief Sets the current position of the entity.
    /// @param position A Vector2f representing the new position.
    void setCurrentPosition(sf::Vector2f position);

    /// @brief Checks if the entity is colliding with a given rectangle.
    /// @param rect A FloatRect representing the bounding box to check against.
    /// @return True if the entity's bounding box intersects with the provided rectangle and false otherwise.
    bool isCollidingWith(const sf::FloatRect &rect);

    /// @brief Pure virtual function to apply damage to the entity.
    /// @param damage The amount of damage to be applied.
    /// @note This method must be overridden by derived classes to define their specific damage behavior.
    virtual void takeDamage(int damage) = 0;
};
