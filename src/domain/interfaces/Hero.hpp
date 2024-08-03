#ifndef HERO_HPP
#define HERO_HPP
#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include <memory>
#include "RangedWeapon.hpp"
#include <iostream>

/// @brief A class that represents the main character, known as "Hero"
class Hero : public Character
{
protected:
    sf::RectangleShape shape;
    std::shared_ptr<RangedWeapon> weapon;

public:
    Hero(float width, float height, float speed, int maxLife, float posX, float posY)
        : Character(width, height, speed, maxLife, posX, posY)
    {
        shape.setFillColor(sf::Color::Cyan);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(posX, posY);
        weapon = std::make_shared<RangedWeapon>(10, 5, 50);
    };

    void move(float deltaTime = {}) override;
    sf::RectangleShape getShape()
    {
        return shape;
    }
    std::shared_ptr<RangedWeapon> getRangedWeapon()
    {
        return this->weapon;
    }
    void doAttack(sf::Vector2f &target) override
    {
        // Get hero position
        auto heroPosition = sf::Vector2f(this->positionX, this->positionY);

        // Set mouse position as target on ranged weapon and do attack
        this->weapon->shoot(target, heroPosition);
    }
};

#endif