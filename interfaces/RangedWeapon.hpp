#pragma once
#include "Weapon.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <SFML/Audio.hpp>

/// @brief A class representing ranged weapons, such as bows and catalysts (staffs). Derived from the Weapon class.
/// @details It adds important functionalities to ranged weaponry, including managing ammunition,
/// launching projectiles and handling firing sounds.
class RangedWeapon : public Weapon
{
private:
    /// @brief Current amount of ammo available for the weapon.
    int ammo;

    /// @brief Maximum amount of ammo the weapon can hold.
    int maxAmmo;

    /// @brief Creates a projectile when the weapon is fired.
    /// @param isHero Boolean indicating if the projectile is from the hero.
    /// @return A shared pointer to the newly created projectile.
    /// @details Calculates the direction and creates a projectile instance with specified damage and velocity.
    std::shared_ptr<Projectile> launchProjectile(bool isHero);

    /// @brief Current position of the weapon (often the character's position).
    sf::Vector2f currentPosition;

    /// @brief Target position for the projectile.
    sf::Vector2f target;

    /// @brief List of projectiles that have been launched by this weapon and are currently on screen.
    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> launchedProjectiles;

    /// @brief Executes an attack with the weapon.
    /// @param isHero Boolean indicating if the weapon is used by the hero.
    /// @note Only performs the attack if there is remaining ammo and the weapon's release time has elapsed.
    void doAttack(bool isHero) override;

    /// @brief Sound played when the weapon is fired with a projectile.
    std::unique_ptr<sf::Sound> arrowSound;

    /// @brief Sound played when a spell is cast.
    std::unique_ptr<sf::Sound> spellSound;

public:
    /// @brief Constructs a `RangedWeapon` object.
    /// @param range Effective range of the weapon.
    /// @param releaseTime Time between consecutive attacks.
    /// @param ammo Initial amount of ammo.
    /// @param damage Damage dealt by each projectile.
    /// @details Initializes the `RangedWeapon` with specified parameters and sets up sound effects for firing.
    RangedWeapon(int range, float releaseTime, int ammo, int damage);

    /// @brief Default RangedWeapon destructor.
    virtual ~RangedWeapon() = default;

    /// @brief Retrieves the list of projectiles that have been launched and are currently on screen.
    /// @return A shared pointer to a list of projectiles.
    std::shared_ptr<std::list<std::shared_ptr<Projectile>>> getLaunchedProjectiles();

    /// @brief Gets the current amount of ammo.
    /// @return The current weapon's ammo.
    int getAmmo() const;

    /// @brief Adds ammo to the weapon's inventory.
    /// @param ammo Amount of ammo.
    /// @note If the amount of ammo would result in the weapon's ammo exceeding the maximum, the weapon's ammo is capped
    /// at the maximum.
    void addAmmo(int ammo);

    /// @brief Sets the current position of the weapon.
    /// @param position New position of the weapon.
    void setCurrentPosition(const sf::Vector2f &position);

    /// @brief Sets the target position for the projectile.
    /// @param target Position where the projectile is aimed.
    void setTarget(const sf::Vector2f &target);

    /// @brief Initiates an attack with the ranged weapon.
    /// @param target Position where the projectile is aimed.
    /// @param currentPosition Current position of the weapon or character.
    /// @param isHero Boolean indicating if the projectile is from the hero.
    /// @details Calls the `doAttack` method to fire a projectile if the weapon is ready and has ammo.
    void shoot(sf::Vector2f &target, sf::Vector2f &currentPosition, bool isHero);

    /// @brief Checks if the weapon is ready to attack based on release time.
    /// @return True if the weapon can attack or false otherwise.
    bool isReadyToAttack() override;
};