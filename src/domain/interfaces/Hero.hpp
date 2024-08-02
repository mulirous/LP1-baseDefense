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
    void doAttack() override
    {
        // Get hero position
        auto heroPosition = sf::Vector2f(this->positionX, this->positionY);

        // Get mouse position when it's clicked
        auto mousePosition = sf::Mouse::getPosition();
        auto target = static_cast<sf::Vector2f>(mousePosition);
        std::cout << "mouse position: " << target.x << " e " << target.y << "\n";

        // Set mouse position as target on ranged weapon and do attack
        this->weapon->shoot(target, heroPosition);
    }
};

#endif