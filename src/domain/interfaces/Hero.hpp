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
    sf::RectangleShape shape;
    std::shared_ptr<RangedWeapon> weapon;
    sf::Vector2f targetPosition; // Nova posição alvo

public:
    Hero(float width, float height, float speed, int maxLife, float posX, float posY)
        : Character(width, height, speed, maxLife, posX, posY)
    {
        shape.setFillColor(sf::Color::Cyan);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(posX, posY);
        weapon = std::make_shared<RangedWeapon>(10, 5, 50);
    };
    std::shared_ptr<RangedWeapon> getRangedWeapon()
    {
        return this->weapon;
    }
    sf::RectangleShape getShape() { return this->shape; }
    void setTargetPosition(sf::Vector2f target) { this->targetPosition = target; }

    void move(float deltaTime = {}) override;
    void doAttack(sf::Vector2f &target) override;
};

#endif