#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
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
    Character(float width, float height, float speed, int maxLife, float posX, float posY);
    virtual ~Character() = default;
    float getWidth() { return width; }
    float getHeigth() { return height; }
    float getSpeed() { return speed; }
    int getMaxLife() { return maximumLife; }
    int getLife() { return currentLife; }
    float getPosX() { return positionX; }
    float getPosY() { return positionY; }

    bool isCollidingWith(std::shared_ptr<Character> other) const
    {
        return this->positionX<other->positionX + other->width &&this->positionX + this->width> other->positionX &&
               this->positionY<other->positionY + other->height &&this->positionY + this->height> other->positionY;
    }

    void resolveCollision(std::shared_ptr<Character> other)
    {
        float overlapX = (this->width / 2 + other->width / 2) - std::abs(this->positionX - other->positionX);
        float overlapY = (this->height / 2 + other->height / 2) - std::abs(this->positionY - other->positionY);

        if (overlapX < overlapY)
        {
            if (this->positionX < other->positionX)
                this->positionX -= overlapX / 2;
            else
                this->positionX += overlapX / 2;

            if (this->positionX < other->positionX)
                other->positionX += overlapX / 2;
            else
                other->positionX -= overlapX / 2;
        }
        else
        {
            if (this->positionY < other->positionY)
                this->positionY -= overlapY / 2;
            else
                this->positionY += overlapY / 2;

            if (this->positionY < other->positionY)
                other->positionY += overlapY / 2;
            else
                other->positionY -= overlapY / 2;
        }
    }

    virtual void doAttack() = 0;

    virtual void move(float deltaTime = {}) = 0;
};

#endif