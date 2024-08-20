#include "../interfaces/Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Enemy::Enemy(float width, float height, float speed, int maxLife, float x, float y, float cX, float cY)
    : Character(width, height, speed, maxLife, x, y)
{
    targetPosition = {cX, cY};
    weapon = std::make_shared<RangedWeapon>(5, 2, 50);
    state = EnemyState::ALIVE;
    int num = getRandomNumber(0, 100);
    drop = num >= 60 ? true : false;
    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();
    sprite->setTexture(*ResourceManager::getTexture(ENEMY_WALK_IMAGE));
    sprite->setScale(2.0f, 2.0f);
    initAnimations();
};

Enemy::Enemy(float width, float height, float speed, int maxLife, sf::Vector2f position, float cX, float cY)
    : Enemy(width, height, speed, maxLife, position.x, position.y, cX, cY) {};
Enemy::Enemy(float width, float height, float speed, int maxLife, sf::Vector2f position, sf::Vector2f target)
    : Enemy(width, height, speed, maxLife, position.x, position.y, target.x, target.y) {};

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

float Enemy::getTimeSinceDeath()
{
    return clockDeath.getElapsedTime().asSeconds();
}

bool Enemy::hasDrop()
{
    return this->drop;
}

std::shared_ptr<RangedWeapon> Enemy::getRangedWeapon()
{
    return this->weapon;
}

void Enemy::initAnimations()
{
    std::cout << "inside Enemy initAnimation\n";
    auto walk = std::make_shared<Animation>(ResourceManager::getTexture(ENEMY_WALK_IMAGE), sf::Vector2u(8, 1), 0.05f);
    (*animations)["walk"] = walk;
}

void Enemy::updateAnimation(const std::string &action, float dt)
{
    (*animations)[action]->update(dt, direction);
    sprite->setTextureRect((*animations)[action]->textureRect);
}

void Enemy::move(float deltaTime)
{
    sf::Vector2f movement(0, 0);

    float directionX = targetPosition.x - this->currentPosition.x;
    float directionY = targetPosition.y - this->currentPosition.y;
    float magnitude = sqrt(pow(directionX, 2) + pow(directionY, 2));

    if (targetPosition.x < currentPosition.x)
    {
        movement.x -= this->speed * deltaTime;
    }
    else if (targetPosition.x > currentPosition.x)
    {
        movement.x += this->speed * deltaTime;
    }

    if (targetPosition.y < currentPosition.y)
    {
        movement.y -= this->speed * deltaTime;
    }
    else if (targetPosition.y > currentPosition.y)
    {
        movement.y += this->speed * deltaTime;
    }

    if (directionX < 1 && directionY < 1 && directionX > -1 && directionY > -1)
    {
        // Hero is on target (or closely enough)
        updateAnimation("idle", deltaTime);
    }
    else
    {
        // Hero isn't close to target, so it will move
        if (movement.x > 0)
        {
            this->direction = CharacterDirection::RIGHT;
        }
        else
        {
            this->direction = CharacterDirection::LEFT;
        }
        updateAnimation("walk", deltaTime);
    }
    sprite->move(movement);

    if (magnitude > 0.5f)
    {
        this->currentPosition.x += (directionX / magnitude) * this->speed * deltaTime;
        this->currentPosition.y += (directionY / magnitude) * this->speed * deltaTime;
        sprite->setPosition(this->currentPosition);

        // Atualizar a animação enquanto o inimigo se move
        updateAnimation("walk", deltaTime);
    }
}

void Enemy::doAttack(sf::Vector2f &target, float dt)
{
    auto current = sf::Vector2f(this->currentPosition);
    this->weapon->shoot(target, current, false);
}
