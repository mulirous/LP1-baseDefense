#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include "RangedWeapon.hpp"
#include "../enums/EnemyState.h"
#include "Character.hpp"
#include "Drop.hpp"
#include <random>
#include <iostream>

/// @brief A class that represents the other characters, known as "Enemy"
class Enemy : public Character
{
protected:
    sf::CircleShape shape;
    sf::Sprite sprite;
    std::shared_ptr<RangedWeapon> weapon;
    /// @brief Flag indicating enemy's state.
    /// @note It's useful to not erase enemy from memory imediatelly, keeping its resources (like drop and projectiles) for a while.
    EnemyState state;
    /// @brief Clock to count how many time has passed since enemy's death
    sf::Clock clockDeath;
    /// @brief Determines if enemy has drop after its death
    bool drop;

public:
    float centerX;
    float centerY;

    Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY)
        : Character(width, height, speed, maxLife, x, y), centerX(cX), centerY(cY)
    {
        shape.setFillColor(sf::Color::Red);
        shape.setRadius(width / 2);
        shape.setPosition(this->currentPosition);
        weapon = std::make_shared<RangedWeapon>(5, 2, 50);
        state = EnemyState::ALIVE;
        std::random_device rd; std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 100);
        if (dist(gen) >= 60)
            drop = true;
        else
            drop = false;
    };

    /// @brief Check if enemy's state is DEAD
    bool isDead() { return this->state == EnemyState::DEAD; }
    /// @brief Sets enemy's state to DEAD
    void kill()
    {
        this->state = EnemyState::DEAD;
        clockDeath = sf::Clock();
        clockDeath.restart();
    }
    float getTimeSinceDeath() { return clockDeath.getElapsedTime().asSeconds(); }
    bool hasDrop() { return this->drop; }

    sf::CircleShape getShape() { return this->shape; }
    std::shared_ptr<RangedWeapon> getRangedWeapon() { return this->weapon; }
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;
    virtual sf::Sprite &getSprite() override;
};

#endif