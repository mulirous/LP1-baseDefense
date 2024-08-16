#include "Animation.hpp"
#include <iostream>
#include <stdlib.h>

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
{
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    totalTime = 0;
    current.x = 0; // Accessing the first texture on row

    // Calculates the size of each texture
    textureRect.width = texture->getSize().x / float(imageCount.x);
    textureRect.height = texture->getSize().y / float(imageCount.y);
};

void Animation::update(float dt)
{
    current.y = 1;
    totalTime += dt;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime; // Has a smoother animation this way
        current.x++;             // Switches to next frame

        // When reaches the end of row, go back to start
        if (current.x >= imageCount.x)
        {
            current.x = 0;
        }
    }
    textureRect.left = current.x * textureRect.width;
}

void Animation::update(float dt, CharacterDirection direction)
{
    update(dt);

    // Flip or not the character
    if (direction == CharacterDirection::RIGHT)
    {
        textureRect.left = current.x * textureRect.width;
        textureRect.width = std::abs(textureRect.width);
    }
    else
    {
        textureRect.left = (current.x + 1) * std::abs(textureRect.width);
        textureRect.width = -std::abs(textureRect.width);
    }
}