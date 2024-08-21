#include "../interfaces/Weapon.hpp"

Weapon::Weapon(int range, float releaseTime, int damage) : range(range), releaseTime(releaseTime), damage(damage)
{
    sf::Clock newClock;
    releaseTimeCounter = newClock;
}

float Weapon::getReleaseTime()
{
    return releaseTime;
}

int Weapon::getDamage()
{
    return damage;
}