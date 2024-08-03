#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include "Character.hpp"

/// @brief A class that represents the other characters, known as "Enemy"
class Enemy : public Character
{
protected:
    sf::CircleShape shape;

public:
    float centerX;
    float centerY;
    const sf::CircleShape &getShape() const { return this->shape; }

    Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY)
        : Character(width, height, speed, maxLife, x, y), centerX(cX / 2), centerY(cY / 2)
    {
        shape.setFillColor(sf::Color::Red);
        shape.setRadius(width / 2);
        shape.setPosition(this->positionX, this->positionY);
    };
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;
};

#endif