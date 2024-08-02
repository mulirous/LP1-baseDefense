#ifndef RANGED_WEAPON_HPP
#define RANGED_WEAPON_HPP
#include "Weapon.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>

/// @brief A class that represents all ranged weapons, like bows.
class RangedWeapon : public Weapon
{
private:
    /// @brief Weapon's ammo
    int ammo;
    /// @brief Creates a projectile when character shoot
    /// @param position
    /// @return A weak pointer to projectile
    std::shared_ptr<Projectile> launchProjectile(const sf::Vector2f &position);
    /// @brief Current position
    sf::Vector2f currentPosition;
    /// @brief Target's position
    sf::Vector2f target;
    /// @brief Launched projectiles of weapon on screen
    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> launchedProjectiles;

    void doAttack() override;

public:
    RangedWeapon(int range, float releaseTime, int ammo) : Weapon(range, releaseTime), ammo(ammo)
    {
        launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();
    }
    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> getLaunchedProjectiles()
    {
        return this->launchedProjectiles;
    }
    void setCurrentPosition(const sf::Vector2f &position)
    {
        this->currentPosition = position;
    }
    int getAmmo()
    {
        return this->ammo;
    }
    void setTarget(const sf::Vector2f &target)
    {
        this->target = target;
    }

    /// @note ENEMY SHOULD SET A DIFFERENT TARGET THAN THE HERO
    /// @param target A vector
    void shoot(sf::Vector2f &target, sf::Vector2f &currentPosition);
};
#endif