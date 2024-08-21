#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <map>
#include "../enums/CharacterDirection.h"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include "Projectile.hpp"
#include "../enums/CharacterAnimation.h"
#include "../src/modules/animation/src/Animation.hpp"

/// @brief An abstract class to serve as base to Hero and Enemy
class Character
{
protected:
    /// @brief Width on pixels
    float width;
    /// @brief Height on pixels
    float height;
    /// @brief How many pixels character moves in a single movement
    float speed;
    /// @brief Max life of character
    int maximumLife;
    /// @brief Current life of character
    int currentLife;
    /// @brief Position of character on screen
    sf::Vector2f currentPosition;
    std::shared_ptr<sf::Sprite> sprite;
    CharacterDirection direction;
    /// @brief Represents current animation
    CharacterAnimation animationState;
    std::shared_ptr<std::map<std::string, std::shared_ptr<Animation>>> animations;

    virtual void initAnimations() = 0;

public:
    Character(float width, float height, float speed, int maxLife, float posX, float posY);
    Character(float width, float height, float speed, int maxLife, sf::Vector2f position);
    virtual ~Character() = default;
    float getWidth();
    float getHeigth();
    float getSpeed();
    int getMaxLife();
    int getLife();
    std::shared_ptr<sf::Sprite> getSprite();
    CharacterAnimation getAnimationState() const;
    sf::Vector2f getCurrentPosition();
    /// @brief Get bounds for character
    /// @return A FloatRect object that represents bounds
    sf::FloatRect getGlobalBounds();
    void setCurrentPosition(sf::Vector2f &position);
    void setAnimationState(CharacterAnimation &animation);
    bool isAnimationCompleted(const std::string &action);
    bool isCollidingWith(const sf::FloatRect &rect);
    bool isCollidingWith(std::shared_ptr<Character> other);
    void resolveCollision(std::shared_ptr<Character> other);
    virtual void takeDamage(int damage) = 0;
    virtual void doAttack(sf::Vector2f &target, float dt = {}) = 0;
    virtual void move(float deltaTime = {}) = 0;
};
