#pragma once

#include <SFML/Graphics.hpp>

/// @file Weapon.hpp
/// @class Weapon
/// @brief Abstract class that represents a generic weapon in the game. All types of Weapon inherits from this.
/// @details This class is designed to be inherited by specific weapon types.
/// It encapsulates common attributes and methods for weapons, such as range, release time, and damage.
/// Also, it handles the timing of attacks and checking if the weapon is ready to attack.
class Weapon
{
protected:
    /// @brief The range of the weapon, indicating how far it can affect.
    int range;

    /// @brief Time in seconds between attacks, used to control attack rate.
    float releaseTime;

    /// @brief Clock used to measure the time since the last attack.
    sf::Clock releaseTimeCounter;

    /// @brief The damage dealt by the weapon.
    int damage;

public:
    /// @brief Constructs a Weapon with specified attributes.
    /// @param range The range of the weapon.
    /// @param releaseTime Time in seconds between attacks.
    /// @param damage The amount of damage dealt by the weapon.
    Weapon(int range, float releaseTime, int damage);

    /// @brief Default Weapon destructor.
    virtual ~Weapon() = default;

    /// @brief Gets the time in seconds between attacks.
    /// @return The release time in seconds.
    float getReleaseTime() const;

    /// @brief Gets the damage dealt by the weapon.
    /// @return The damage amount.
    int getDamage() const;

    /// @brief Makes an attack.
    /// @param isHero Boolean indicating if who's attacking is hero or not. Default is `false`.
    /// @note This is a pure virtual function and must be implemented by derived classes to define specific attack behavior.
    virtual void doAttack(bool isHero = false) = 0;

    /// @brief Checks if the weapon is ready to attack based on the release time.
    /// @return True if the weapon is ready to attack or false otherwise.
    virtual bool isReadyToAttack() = 0;
};
