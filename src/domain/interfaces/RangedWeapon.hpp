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
    /// @return A pointer to projectile
    std::shared_ptr<Projectile> launchProjectile();
    /// @brief Current position
    sf::Vector2f currentPosition;
    /// @brief Target's position
    sf::Vector2f target;
    /// @brief Launched projectiles of weapon on screen
    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> launchedProjectiles;

    /// @brief Makes an attack
    /// @note Only attack if there is remaining ammo and if the weapon's release time has passed
    /// @returns void
    void doAttack() override;

public:
    RangedWeapon(int range, float releaseTime, int ammo) : Weapon(range, releaseTime), ammo(ammo)
    {
        launchedProjectiles = std::make_shared<std::list<std::shared_ptr<Projectile>>>();
    }

    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> getLaunchedProjectiles() { return this->launchedProjectiles; }
    int getAmmo() { return this->ammo; }
    void addAmmo(int ammo)
    {
        if (this->ammo + ammo > 100)
            return;
        this->ammo += ammo;
    }
    void setCurrentPosition(const sf::Vector2f &position) { this->currentPosition = position; }
    void setTarget(const sf::Vector2f &target) { this->target = target; }

    /// @brief Represents the ranged weapon's attack.
    /// @param target Target's position on Vector2f type
    /// @param currentPosition Character's current position on Vector2f type
    /// @note Internally calls Weapon->doAttack method
    void shoot(sf::Vector2f &target, sf::Vector2f &currentPosition);
    bool isReadyToAttack() override;
};
#endif