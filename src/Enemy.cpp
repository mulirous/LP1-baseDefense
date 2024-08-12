#include "../interfaces/Enemy.hpp"
#include <cmath>

Enemy::Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY)
    : Character(width, height, speed, maxLife, x, y), centerX(cX), centerY(cY)
{
    shape.setFillColor(sf::Color::Red);
    shape.setRadius(width / 2);
    shape.setPosition(this->currentPosition);
    weapon = std::make_shared<RangedWeapon>(5, 2, 50);
    state = EnemyState::ALIVE;
    int num = getRandomNumber(0, 100);

    drop = num >= 60 ? true : false;
};

bool Enemy::isDead()
{
    return this->state == EnemyState::DEAD;
}

void Enemy::kill()
{
    this->state = EnemyState::DEAD;
    clockDeath = sf::Clock();
    clockDeath.restart();
}

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

sf::Sprite &Enemy::getSprite()
{
    return sprite;
}