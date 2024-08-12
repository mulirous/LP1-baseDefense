#ifndef BASE_HPP
#define BASE_HPP

#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "../common.h"

/// @brief Represents the base which hero must defend and enemies will attack
class Base
{
private:
    /// @brief Base's max life
    int maxLife;
    /// @brief Base's current life
    int currentLife;
    /// @brief Base's position on screen
    sf::Vector2f currentPosition;
    /// @brief Pointer to base's sprite
    std::shared_ptr<sf::Sprite> baseSprite;

public:
    /// @brief Default constructor.
    /// @param maxLife Base's max life
    /// @param posX
    /// @param posY
    /// @param texture
    Base(int maxLife, float posX, float posY);
    virtual ~Base() = default;
    std::shared_ptr<sf::Sprite> getSprite();
    sf::Vector2f getPosition();
    void takeDamage(int damage);
    int getLife();
    bool isCollidingWith(const sf::FloatRect &other);
};
#endif // ! BASE_HPP