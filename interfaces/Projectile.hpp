#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include "../common.h"
#include <map>
#include <list>
#include "../src/modules/animation/src/Animation.hpp"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"

class Projectile
{
private:
    /// @brief Projectile's current position
    sf::Vector2f position;
    /// @brief Projectile's target
    sf::Vector2f target;
    /// @brief Projectile's velocity
    float velocity;
    /// @brief Projectile's damage
    int damage;
    /// @brief The sprite used to render the projectile
    std::shared_ptr<sf::Sprite> sprite;
    /// @brief Animation Manager
    std::shared_ptr<std::map<std::string, std::shared_ptr<Animation>>> animations;
    /// @brief Initializes projectile animations
    void initAnimations();

public:
    Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target, bool isHero);
    const sf::Sprite &getSprite();
    sf::FloatRect getBounds();
    int getDamage();
    void update(float deltaTime);
    bool isOffScreen() const;
    bool hasReachedTarget() const;
};
