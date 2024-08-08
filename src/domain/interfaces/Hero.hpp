#ifndef HERO_HPP
#define HERO_HPP
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include <memory>
#include "RangedWeapon.hpp"

/// @brief A class that represents the main character, known as "Hero"
class Hero : public Character
{
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i frameSize;
    int currentFrame;
    float frameTime;
    float timeSinceLastFrame;

    /// @brief Pointer to hero's ranged weapon
    std::shared_ptr<RangedWeapon> weapon;
    /// @brief Position that hero will move to
    sf::Vector2f targetPosition;

public:
    Hero(float width, float height, float speed, int maxLife, float posX, float posY)
        : Character(width, height, speed, maxLife, posX, posY), currentFrame(0), frameTime(0.1f), timeSinceLastFrame(0.0f)
    {

        texture.loadFromFile("src/sprites/hero.png");
        sprite.setTexture(texture);
        frameSize = sf::Vector2i(width, height);
        sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
        sprite.setPosition(posX, posY);
        weapon = std::make_shared<RangedWeapon>(10, 0.5, 50);

          // Escala a sprite para reduzir o tamanho
        float scaleFactor = 1.5f; // diminui para 60% do tamanho original
        sprite.setScale(scaleFactor, scaleFactor);
    };

    void updateAnimation(float deltaTime);
    sf::Sprite& getSprite() override { return sprite; } // Implement the pure virtual function
    sf::Vector2f getPosition();

    std::shared_ptr<RangedWeapon> getRangedWeapon() { return this->weapon; }
    void setTargetPosition(sf::Vector2f target) { this->targetPosition = target; }

    void takeDamage(int damage);
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;
};

#endif