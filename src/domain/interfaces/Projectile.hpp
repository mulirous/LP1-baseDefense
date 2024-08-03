#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include "../common.h"

class Projectile
{
private:
    /// @brief Projectile's current position
    sf::Vector2f position;
    /// @brief Projectile's target
    sf::Vector2f target;
    /// @brief Projectile's velocity
    float velocity;
    /// @brief Projectile's damage
    int damage;
    /// @brief
    sf::CircleShape shape;

public:
    Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target)
        : position(position), damage(damage), velocity(velocity), target(target)
    {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(this->position);
        shape.setRotation(atan2(target.y, target.x) * 180 / 3.14159265);
    }

    const sf::CircleShape &getShape() const
    {
        return this->shape;
    }

    void update(float deltaTime)
    {
        shape.move(target * velocity);
    }

    sf::FloatRect getBounds() const
    {
        return shape.getGlobalBounds();
    }

    bool isOffScreen() const
    {
        sf::FloatRect bounds = shape.getGlobalBounds();
        return (bounds.left + bounds.width < 0 || bounds.left > GameWindowWidth ||
                bounds.top + bounds.height < 0 || bounds.top > GameWindowHeight);
    }
};

#endif
