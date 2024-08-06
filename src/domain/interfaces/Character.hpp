#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <iostream>
#include "Projectile.hpp"

/// @brief An abstract class to serve as base to Hero and Enemy
class Character
{
protected:
    /// @brief Width on pixels
    float width;
    /// @brief Height on pixels
    float height;
    /// @brief How many pixels character moves in a single movement
    float speed;
    /// @brief Max life of character
    int maximumLife;
    /// @brief Current life of character
    int currentLife;
    /// @brief X-axis position of character on screen
    float positionX;
    /// @brief Y-axis position of character on screen
    float positionY;

public:
    Character(float width, float height, float speed, int maxLife, float posX, float posY) : width(width), height(height), speed(speed),
                                                                                             maximumLife(maxLife), currentLife(maxLife), positionX(posX), positionY(posY) {};
    virtual ~Character() = default;
    float getWidth() { return width; }
    float getHeigth() { return height; }
    float getSpeed() { return speed; }
    int getMaxLife() { return maximumLife; }
    int getLife() { return currentLife; }
    float getPosX() { return positionX; }
    float getPosY() { return positionY; }

    bool isCollidingWith(const sf::FloatRect &rect)
    {
        return this->getGlobalBounds().intersects(rect);
    }

    bool isCollidingWith(std::shared_ptr<Character> other)
    {
        return this->getGlobalBounds().intersects(other->getGlobalBounds());
    }

    sf::FloatRect getGlobalBounds()
    {
        return sf::FloatRect(this->positionX, this->positionY, this->width, this->height);
    }

    void resolveCollision(std::shared_ptr<Character> other);

    virtual void doAttack(sf::Vector2f &target) = 0;

    virtual void move(float deltaTime = {}) = 0;
};

#endif