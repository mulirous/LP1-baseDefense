#include "../interfaces/Weapon.hpp"

Weapon::Weapon(int range, float releaseTime, int damage) : range(range), releaseTime(releaseTime), damage(damage)
{
    releaseTimeCounter.restart(); // Initialize counter
}

float Weapon::getReleaseTime() const
{
    return releaseTime;
}

int Weapon::getDamage() const
{
    return damage;
}