#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "RangedWeapon.hpp"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include "../enums/EnemyState.h"
#include "Character.hpp"
#include "Drop.hpp"
#include <random>
#include <type_traits>
#include <memory>

/// @brief A class that represents the other characters, known as "Enemy"
class Enemy : public Character
{
protected:
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

    // No implementation yet
    float getTimeSinceDeath();
    bool hasDrop();

    void initAnimations() override;
    void updateAnimation(const std::string &action, float dt);

    std::shared_ptr<RangedWeapon> getRangedWeapon();
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target, float dt = {}) override;
};
