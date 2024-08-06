#include "interfaces/Enemy.hpp"
#include <cmath>

void Enemy::move(float deltaTime)
{
    float directionX = centerX - this->currentPosition.x;
    float directionY = centerY - this->currentPosition.y;
    float magnitude = sqrt(pow(directionX, 2) + pow(directionY, 2));

    if (magnitude > 0.5f)
    {
        this->currentPosition.x += (directionX / magnitude) * this->speed * deltaTime;
        this->currentPosition.y += (directionY / magnitude) * this->speed * deltaTime;
        shape.setPosition(this->currentPosition);
    }
}

void Enemy::doAttack(sf::Vector2f &target)
{
    auto current = sf::Vector2f(this->currentPosition);
    this->weapon->shoot(target, current);
}