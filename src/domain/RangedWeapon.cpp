#include "interfaces/RangedWeapon.hpp"
#include <math.h>
#include "common.h"
#include <iostream>

std::shared_ptr<Projectile> RangedWeapon::launchProjectile()
{
    sf::Vector2f direction = this->target - this->currentPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = sf::Vector2f(direction.x / length, direction.y / length);

    return std::make_shared<Projectile>(10, PROJECTILE_VELOCITY, this->currentPosition, direction, "src/sprites/arrow.png");
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