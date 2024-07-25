#include "interfaces/Enemy.hpp"
#include <cmath>
#include <iostream>

// TO-DO: use the move method instead update
void Enemy::update(float deltaTime)
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

void Enemy::move()
{
}