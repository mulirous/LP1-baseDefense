#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <math.h>
#include "../common.h"
#include <iostream>

using namespace std;

class Projectile
{
private:
    sf::Vector2f position;
    sf::Vector2f target;
    float velocity;
    int damage;
    sf::Sprite sprite;
    sf::Texture texture;
    int spriteFrameWidth;
    int spriteFrameHeight;
    int currentFrame;
    float animationTime;
    float elapsedTime;

public:
    Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target, const std::string& texturePath)
        : position(position), damage(damage), velocity(velocity), target(target),
          currentFrame(0), animationTime(0.1f), elapsedTime(0.0f)
    {
        if (!texture.loadFromFile(texturePath))
        {
            cout << "Failed to load projectile texture. " << endl;
        }

        sprite.setTexture(texture);
        spriteFrameWidth = texture.getSize().x / 4; // Assuming 4 frames horizontally
        spriteFrameHeight = texture.getSize().y; // Full height
        sprite.setTextureRect(sf::IntRect(0, 0, spriteFrameWidth, spriteFrameHeight));
        sprite.setPosition(this->position);

        float scaleFactor = 0.09f; // diminui para 60% do tamanho original
        sprite.setScale(scaleFactor, scaleFactor);
    }

    sf::Sprite& getSprite() { return this->sprite; }
    sf::FloatRect getBounds() { return sprite.getGlobalBounds(); }
    int getDamage() { return this->damage; }

    void update(float deltaTime)
    {
        elapsedTime += deltaTime;
        if (elapsedTime >= animationTime)
        {
            elapsedTime = 0;
            currentFrame = (currentFrame + 1) % 4; // Cycle through 4 frames
            sprite.setTextureRect(sf::IntRect(currentFrame * spriteFrameWidth, 0, spriteFrameWidth, spriteFrameHeight));
        }

        sf::Vector2f direction = target - position;
        float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= magnitude;

        position += direction * velocity * deltaTime;
        sprite.setPosition(position);
    }

    bool isOffScreen() const
    {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        return (bounds.left + bounds.width < 0 || bounds.left > GAMEWINDOWWIDTH ||
                bounds.top + bounds.height < 0 || bounds.top > GAMEWINDOWHEIGHT);
    }
};

#endif
