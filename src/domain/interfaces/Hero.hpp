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
    sf::RectangleShape getShape()
    {
        return this->shape;
    }
    std::shared_ptr<RangedWeapon> getRangedWeapon()
    {
        return this->weapon;
    }

    void move(float deltaTime = {}) override;
    void doAttack(sf::Vector2f &target) override;
};

#endif