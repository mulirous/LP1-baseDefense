#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include "Projectile.hpp"

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

public:
    Character(float width, float height, float speed, int maxLife, float posX, float posY, std::string filepath);
    Character(float width, float height, float speed, int maxLife, sf::Vector2f position, std::string filepath);
    virtual ~Character() = default;
    float getWidth();
    float getHeigth();
    float getSpeed();
    int getMaxLife();
    int getLife();
    sf::Vector2f getCurrentPosition();
    /// @brief Get bounds for character
    /// @return A FloatRect object that represents bounds
    sf::FloatRect getGlobalBounds();
    void setCurrentPosition(sf::Vector2f &position);
    bool isCollidingWith(const sf::FloatRect &rect);
    bool isCollidingWith(std::shared_ptr<Character> other);
    void resolveCollision(std::shared_ptr<Character> other);
    virtual void doAttack(sf::Vector2f &target) = 0;
    virtual void move(float deltaTime = {}) = 0;
    std::shared_ptr<sf::Sprite> getSprite();
};
