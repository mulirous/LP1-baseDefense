#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "RangedWeapon.hpp"
#include "../enums/EnemyState.h"
#include "Character.hpp"
#include "Drop.hpp"
#include <random>

/// @brief A class that represents the other characters, known as "Enemy"
class Enemy : public Character
{
protected:
    sf::CircleShape shape;
    std::shared_ptr<RangedWeapon> weapon;
    /// @brief
    sf::Vector2f targetPosition;
    /// @brief Flag indicating enemy's state.
    /// @note It's useful to not erase enemy from memory imediatelly, keeping its resources (like drop and projectiles) for a while.
    EnemyState state;
    /// @brief Clock to count how many time has passed since enemy's death
    sf::Clock clockDeath;
    /// @brief Determines if enemy has drop after its death
    bool drop;

public:
    Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY);
    Enemy(float width, float height, float speed, int maxLife, sf::Vector2f position, float cX, float cY);
    Enemy(float width, float height, float speed, int maxLife, sf::Vector2f position, sf::Vector2f target);

    /// @brief Check if enemy's state is DEAD
    bool isDead();

    /// @brief Sets enemy's state to DEAD
    void kill();

    float getTimeSinceDeath();
    bool hasDrop();

    sf::CircleShape getShape();
    std::shared_ptr<RangedWeapon> getRangedWeapon();
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;
};
