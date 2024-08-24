#include "Animation.hpp"
#include <stdlib.h>

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, std::unique_ptr<std::vector<sf::IntRect>> framesSizes)
{
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    totalTime = 0;
    current.x = 0; // Accessing the first texture on row

    if (framesSizes && !framesSizes->empty())
    {
        this->framesSizes = std::move(framesSizes);
    }
    else
    {
        textureRect.width = texture->getSize().x / float(imageCount.x);
        textureRect.height = texture->getSize().y / float(imageCount.y);

        // Creates frames dynamically
        this->framesSizes = std::make_unique<std::vector<sf::IntRect>>();
        for (int y = 0; y < imageCount.y; ++y)
        {
            for (int x = 0; x < imageCount.x; ++x)
                this->framesSizes->emplace_back(x * textureRect.width, y * textureRect.height, textureRect.width, textureRect.height);
        }
    }
};

void Animation::reset()
{
    current.x = 0;
}

bool Animation::isCompleted() const
{
    return current.x + 1 == imageCount.x;
}

void Animation::update(float dt)
{
    current.y = 1;
    totalTime += dt;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime; // Has a smoother animation this way
        current.x++;             // Switches to next frame

        // When reaches the end of row, go back to start
        if (current.x >= framesSizes->size())
            current.x = 0;
    }

    textureRect = (*framesSizes)[current.x]; // Access the current frame's IntRect
}

void Animation::update(float dt, CharacterDirection direction)
{
    update(dt);

    // Flip or not the character based on direction
    if (direction == CharacterDirection::RIGHT)
    {
        textureRect = (*framesSizes)[current.x]; // Normal frame rendering
    }
    else
    {
        textureRect = (*framesSizes)[current.x];
        textureRect.left += textureRect.width;  // Adjust the left position
        textureRect.width = -textureRect.width; // Invert the width for flipping
    }
}
