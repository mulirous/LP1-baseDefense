#pragma once

#include <SFML/Graphics.hpp>

/// @brief An abstract class that represents a generic weapon
class Weapon
{
protected:
    /// @brief
    int range;
    /// @brief Time in seconds between attacks
    float releaseTime;
    /// @brief
    sf::Clock releaseTimeCounter;
    /// @brief
    int damage;

public:
    Weapon(int range, float releaseTime, int damage);
    virtual ~Weapon() = default;
    float getReleaseTime();
    int getDamage();
    /// @brief Makes an attack
    virtual void doAttack() = 0;
    virtual bool isReadyToAttack() = 0;
};