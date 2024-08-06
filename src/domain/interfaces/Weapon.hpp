#ifndef WEAPON_HPP
#define WEAPON_HPP
#include <SFML/Graphics.hpp>

/// @brief An abstract class that represents a generic weapon
class Weapon
{
protected:
    /// @brief
    int range;
    /// @brief Time in seconds between attacks
    float releaseTime;

public:
    Weapon(int range, float releaseTime) : range(range), releaseTime(releaseTime) {}
    virtual ~Weapon() = default;
    /// @brief Makes an attack
    virtual void doAttack() = 0;
};
#endif