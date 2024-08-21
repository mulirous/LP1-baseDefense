#include "../interfaces/RangedWeapon.hpp"
#include <math.h>
#include "../common.h"
#include <iostream>

RangedWeapon::RangedWeapon(int range, float releaseTime, int ammo, int damage) : Weapon(range, releaseTime, damage), ammo(ammo), maxAmmo(ammo)
{
    launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();
}

std::shared_ptr<Projectile> RangedWeapon::launchProjectile()
{
    sf::Vector2f direction = this->target - this->currentPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = sf::Vector2f(direction.x / length, direction.y / length);

    return std::make_shared<Projectile>(damage, PROJECTILE_VELOCITY, this->currentPosition, direction);
}

void RangedWeapon::addAmmo(int ammo)
{
    if (this->ammo + ammo > maxAmmo)
    {
        this->ammo = maxAmmo;
        return;
    }
    this->ammo += ammo;
}

void RangedWeapon::shoot(sf::Vector2f &target, sf::Vector2f &currentPosition, bool isHero)
{
    setCurrentPosition(currentPosition);
    setTarget(target);

    if (isHero)
    {
        doAttack();
        spellSound.play();
    }
    else
    {
        doAttack();
        arrowSound.play();
    }
}

void RangedWeapon::doAttack()
{
    std::cout << "shooting causes " << damage << " damage!" << std::endl;
    if (ammo == 0 || !isReadyToAttack())
        return;

    // Creates a new projectile and shoot it
    std::shared_ptr<Projectile> newProjectile = launchProjectile();

    launchedProjectiles->push_back(newProjectile);

    ammo--;
    releaseTimeCounter.restart();
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