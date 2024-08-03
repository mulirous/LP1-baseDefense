#include "interfaces/Enemy.hpp"
#include <cmath>
#include <iostream>

void Enemy::move(float deltaTime)
{
    float directionX = centerX - this->positionX;
    float directionY = centerY - this->positionY;
    float magnitude = sqrt(pow(directionX, 2) + pow(directionY, 2));

    if (magnitude > 0.5f)
    {
        this->positionX += (directionX / magnitude) * this->speed * deltaTime;
        this->positionY += (directionY / magnitude) * this->speed * deltaTime;
        shape.setPosition(this->positionX, this->positionY);
    }
}

void Enemy::doAttack(sf::Vector2f &target) {}