#ifndef HERO_HPP
#define HERO_HPP
#include <SFML/Graphics.hpp>
#include "Character.hpp"

/// @brief A class that represents the main character, known as "Hero"
class Hero : public Character
{
protected:
    sf::RectangleShape shape;
    sf::Vector2f targetPosition; // Nova posição alvo

public:
    Hero(float width, float height, float speed, int maxLife, float posX, float posY)
        : Character(width, height, speed, maxLife, posX, posY)
    {
        shape.setFillColor(sf::Color::Cyan);
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(posX, posY);
    };

    void setTargetPosition(sf::Vector2f target) { targetPosition = target; }
    void move();
    sf::RectangleShape getShape() { return shape; }
};

#endif