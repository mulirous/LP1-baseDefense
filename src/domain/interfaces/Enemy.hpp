#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include "RangedWeapon.hpp"
#include "Character.hpp"

/// @brief A class that represents the other characters, known as "Enemy"
class Enemy : public Character
{
protected:
    sf::CircleShape shape;
    std::shared_ptr<RangedWeapon> weapon;
    sf::Sprite sprite;

public:
    float centerX;
    float centerY;
    sf::CircleShape getShape() { return this->shape; }

    Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY)
        : Character(width, height, speed, maxLife, x, y), centerX(cX), centerY(cY)
    {
        shape.setFillColor(sf::Color::Red);
        shape.setRadius(width / 2);
        shape.setPosition(this->currentPosition);
        weapon = std::make_shared<RangedWeapon>(5, 2, 50);
        
    };

    virtual sf::Sprite& getSprite() override;

    std::shared_ptr<RangedWeapon> getRangedWeapon() { return this->weapon; }

    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;
};

#endif