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

    /// @brief
    /// @param dt delta time
    void update(float dt, CharacterDirection direction);
};