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
    sf::Clock releaseTimeCounter;

public:
    Weapon(int range, float releaseTime) : range(range), releaseTime(releaseTime)
    {
        sf::Clock newClock;
        releaseTimeCounter = newClock;
    }
    virtual ~Weapon() = default;
    float getReleaseTime() { return this->releaseTime; }
    /// @brief Makes an attack
    virtual void doAttack() = 0;
    virtual bool isReadyToAttack() = 0;
};
#endif