#include "../interfaces/RangedWeapon.hpp"
#include <cmath>
#include "../common.h"
#include <iostream>

RangedWeapon::RangedWeapon(int range, float releaseTime, int ammo)
    : Weapon(range, releaseTime), ammo(ammo)
{
    launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();

    // Inicialize os buffers de som e sons
    if (!arrowSoundBuffer.loadFromFile(ARROW_MUSIC))
    {
        std::cerr << "Failed to load arrow sound file!" << std::endl;
    }
    if (!spellSoundBuffer.loadFromFile(SPELL_MUSIC))
    {
        std::cerr << "Failed to load spell sound file!" << std::endl;
    }

    arrowSound.setBuffer(arrowSoundBuffer);
    spellSound.setBuffer(spellSoundBuffer);
}

std::shared_ptr<Projectile> RangedWeapon::launchProjectile(bool isHero)
{
    sf::Vector2f direction = this->target - this->currentPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = sf::Vector2f(direction.x / length, direction.y / length);

    return std::make_shared<Projectile>(10, PROJECTILE_VELOCITY, this->currentPosition, direction, isHero);
}

void RangedWeapon::addAmmo(int ammo)
{
    if (this->ammo + ammo > 100)
        return;
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
    if (ammo == 0 || !this->isReadyToAttack())
        return;

    auto newProjectile = this->launchProjectile(isHero);
    this->launchedProjectiles->push_back(newProjectile);
    this->ammo--;
    this->releaseTimeCounter.restart();

    if (isHero)
    {
        spellSound.setVolume(100); // Ajuste o volume se necessário
        spellSound.play();
    }
    else
    {
        arrowSound.setVolume(100); // Ajuste o volume se necessário
        arrowSound.play();
    }
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
