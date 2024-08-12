#include "../interfaces/RangedWeapon.hpp"
#include <math.h>
#include "../common.h"
#include <iostream>

RangedWeapon::RangedWeapon(int range, float releaseTime, int ammo) : Weapon(range, releaseTime), ammo(ammo)
{
    launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();
}

std::shared_ptr<Projectile> RangedWeapon::launchProjectile()
{
    sf::Vector2f direction = this->target - this->currentPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = sf::Vector2f(direction.x / length, direction.y / length);

    return std::make_shared<Projectile>(10, PROJECTILE_VELOCITY, this->currentPosition, direction);
}

void RangedWeapon::addAmmo(int ammo)
{
    if (this->ammo + ammo > 100)
        return;
    this->ammo += ammo;
}

void RangedWeapon::shoot(sf::Vector2f &target, sf::Vector2f &currentPosition)
{
    setCurrentPosition(currentPosition);
    setTarget(target);
    doAttack();
}

void RangedWeapon::doAttack()
{
    if (ammo == 0 || !this->isReadyToAttack())
        return;

    // Creates a new projectile and shoot it
    auto newProjectile = this->launchProjectile();
    this->launchedProjectiles->push_back(newProjectile);
    this->ammo--;
    this->releaseTimeCounter.restart();
}

bool RangedWeapon::isReadyToAttack()
{
    return this->releaseTimeCounter.getElapsedTime().asSeconds() >= this->releaseTime;
}

std::shared_ptr<std::list<std::shared_ptr<Projectile>>> RangedWeapon::getLaunchedProjectiles()
{
    return this->launchedProjectiles;
}

int RangedWeapon::getAmmo()
{
    return this->ammo;
}

void RangedWeapon::setCurrentPosition(const sf::Vector2f &position)
{
    this->currentPosition = position;
}

void RangedWeapon::setTarget(const sf::Vector2f &target)
{
    this->target = target;
}