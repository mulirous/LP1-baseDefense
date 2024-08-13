#pragma once

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
    Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target);
    const sf::CircleShape &getShape();
    sf::FloatRect getBounds();
    int getDamage();
    void update(float deltaTime);
    bool isOffScreen() const;
};
