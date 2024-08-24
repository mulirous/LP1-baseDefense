#include "../interfaces/RangedWeapon.hpp"
#include <cmath>
#include "../common.h"

RangedWeapon::RangedWeapon(int range, float releaseTime, int ammo, int damage) : Weapon(range, releaseTime, damage), ammo(ammo), maxAmmo(ammo)
{
    launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();

    spellSound = std::make_unique<sf::Sound>();
    arrowSound = std::make_unique<sf::Sound>();

    spellSound->setBuffer(*ResourceManager::getSoundBuffer(SPELL_MUSIC));
    spellSound->setVolume(100);

    arrowSound->setBuffer(*ResourceManager::getSoundBuffer(ARROW_MUSIC));
    arrowSound->setVolume(100);
}

std::shared_ptr<Projectile> RangedWeapon::launchProjectile(bool isHero)
{
    sf::Vector2f direction = target - currentPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = sf::Vector2f(direction.x / length, direction.y / length);

    return std::make_shared<Projectile>(damage, PROJECTILE_VELOCITY, currentPosition, currentPosition + direction * 1000.f, isHero);
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
    doAttack(isHero);
}

void RangedWeapon::doAttack(bool isHero)
{
    if (ammo == 0 || !isReadyToAttack())
        return;

    auto newProjectile = launchProjectile(isHero);
    launchedProjectiles->push_back(newProjectile);
    ammo--;
    releaseTimeCounter.restart();

    if (isHero)
        spellSound->play();
    else
        arrowSound->play();
}

bool RangedWeapon::isReadyToAttack()
{
    return releaseTimeCounter.getElapsedTime().asSeconds() >= releaseTime;
}

std::shared_ptr<std::list<std::shared_ptr<Projectile>>> RangedWeapon::getLaunchedProjectiles()
{
    return launchedProjectiles;
}

int RangedWeapon::getAmmo() const
{
    return ammo;
}

void RangedWeapon::setCurrentPosition(const sf::Vector2f &position)
{
    currentPosition = position;
}

void RangedWeapon::setTarget(const sf::Vector2f &target)
{
    this->target = target;
}
