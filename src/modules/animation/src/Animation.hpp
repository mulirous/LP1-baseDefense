#pragma once

#include <SFML/Graphics.hpp>
#include "../../../../enums/CharacterDirection.h"

class Animation
{
private:
    /// @brief Amount of textures on row
    sf::Vector2u imageCount;
    /// @brief Current texture on spritesheet
    sf::Vector2u current;
    /// @brief
    float totalTime;
    /// @brief Time between images
    float switchTime;

public:
    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime);
    virtual ~Animation() = default;
    /// @brief Size of texture
    sf::IntRect textureRect;

    /// @brief Default update function
    /// @param dt
    /// @note It will advances one frame at a time and set textureRect.left as it advances
    void update(float dt);
    /// @brief Specific update function to characters
    /// @param dt delta time
    /// @param direction Direction which character is facing at time
    /// @note Calls default update but modifies textureRect based on character's direction
    void update(float dt, CharacterDirection direction);
};