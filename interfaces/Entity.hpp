#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

/// @brief Abstract class. Must be inherit by Character and Base
class Entity
{
protected:
    sf::Vector2f currentPosition;

    std::shared_ptr<sf::Sprite> sprite;
    int maximumLife;
    int currentLife;

public:
    Entity(int life);
    Entity(int life, float posX, float posY);
    Entity(int life, sf::Vector2f pos);
    virtual ~Entity() = default;

    int getMaxLife();

    int getLife();

    std::shared_ptr<sf::Sprite> getSprite();

    sf::FloatRect getBounds();
    sf::Vector2f getCurrentPosition();

    void setCurrentPosition(sf::Vector2f &position);

    /// @brief Checks if the character is colliding with a given rectangle.
    /// @param rect A FloatRect representing the bounding box to check against.
    /// @return True if the character's bounding box intersects with the provided rectangle.
    bool isCollidingWith(const sf::FloatRect &rect);

    virtual void takeDamage(int damage) = 0;
};