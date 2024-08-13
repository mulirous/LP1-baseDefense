#include "../interfaces/Weapon.hpp"

Weapon::Weapon(int range, float releaseTime) : range(range), releaseTime(releaseTime)
{
    sf::Clock newClock;
    releaseTimeCounter = newClock;
}

float Weapon::getReleaseTime()
{
    return this->releaseTime;
}