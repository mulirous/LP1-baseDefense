#include "interfaces/RangedWeapon.hpp"
#include <math.h>
#include "common.h"
#include <iostream>

std::shared_ptr<Projectile> RangedWeapon::launchProjectile(const sf::Vector2f &position)
{
    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        direction /= length;

    std::cout << "Launched projectile!";

    return std::make_shared<Projectile>(10, 5, position, direction);
}

void RangedWeapon::shoot(sf::Vector2f &target, sf::Vector2f &currentPosition)
{
    std::cout << "entered shoot! \n";
    setCurrentPosition(currentPosition);
    std::cout << "set currentPosition: " << currentPosition.x << ", " << currentPosition.y << "\n";
    setTarget(target);
    std::cout << "set target: " << target.x << ", " << target.y << "\n";
    doAttack();
}

void RangedWeapon::doAttack()
{
    std::cout << "start do attack!\n";
    if (ammo == 0)
    {
        std::cout << "dont have any ammo :( \n";
        return;
    }

    std::cout << "will launch now\n";
    auto newProjectile = this->launchProjectile(this->currentPosition);
    std::cout << "add projectile now\n";
    this->launchedProjectiles->push_back(newProjectile);
    std::cout << "now, projectiles are: " << this->launchedProjectiles->size() << "\n";
}