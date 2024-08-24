#include "../interfaces/Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Enemy::Enemy(float width, float height, float speed, int maxLife, int weaponDamage, float x, float y, float cX, float cY)
    : Character(width, height, speed, maxLife, x, y),
      targetPosition(cX, cY),
      state(EnemyState::ALIVE),
      drop(getRandomNumber(0, 100) >= 60)
{
    weapon = std::make_shared<RangedWeapon>(5, 2, 50, weaponDamage);

    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();
    animationState = CharacterAnimation::WALK;

    sprite->setTexture(*ResourceManager::getTexture(ENEMY_WALK_IMAGE));
    sprite->setScale(2.0f, 2.0f);
    initAnimations();
};

Enemy::Enemy(float width, float height, float speed, int maxLife, int weaponDamage, sf::Vector2f position, float cX, float cY)
    : Enemy(width, height, speed, maxLife, weaponDamage, position.x, position.y, cX, cY) {};
Enemy::Enemy(float width, float height, float speed, int maxLife, int weaponDamage, sf::Vector2f position, sf::Vector2f target)
    : Enemy(width, height, speed, maxLife, weaponDamage, position.x, position.y, target.x, target.y) {};

bool Enemy::isDead()
{
    return this->state == EnemyState::DEAD;
}

void Enemy::takeDamage(int damage)
{
    Character::takeDamage(damage);

    if (currentLife <= 0)
        kill();
}

void Enemy::kill()
{
    state = EnemyState::DEAD;
    clockDeath = sf::Clock();
    clockDeath.restart();
}

float Enemy::getTimeSinceDeath()
{
    return clockDeath.getElapsedTime().asSeconds();
}

bool Enemy::hasDrop()
{
    return drop;
}

void Enemy::initAnimations()
{
    auto attackFrames = std::make_unique<std::vector<sf::IntRect>>(std::initializer_list<sf::IntRect>({sf::IntRect(0, 0, 20, 21),
                                                                                                       sf::IntRect(20, 0, 19, 21),
                                                                                                       sf::IntRect(39, 0, 19, 21),
                                                                                                       sf::IntRect(58, 0, 25, 21),
                                                                                                       sf::IntRect(83, 0, 23, 21),
                                                                                                       sf::IntRect(106, 0, 22, 21),
                                                                                                       sf::IntRect(128, 0, 35, 21),
                                                                                                       sf::IntRect(163, 0, 21, 21),
                                                                                                       sf::IntRect(184, 0, 19, 21)}));
    auto walk = std::make_shared<Animation>(ResourceManager::getTexture(ENEMY_WALK_IMAGE), sf::Vector2u(8, 1), 0.05f);
    auto attack = std::make_shared<Animation>(ResourceManager::getTexture(ENEMY_ATTACK_IMAGE), sf::Vector2u(9, 1), 0.05f, std::move(attackFrames));
    (*animations)["walk"] = walk;
    (*animations)["attack"] = attack;
}

void Enemy::updateAnimation(const std::string &action, float dt)
{
    if (action == "walk" && sprite->getTexture() != ResourceManager::getTexture(ENEMY_WALK_IMAGE) && animationState == CharacterAnimation::WALK)
    {
        sprite->setTexture(*ResourceManager::getTexture(ENEMY_WALK_IMAGE));
    }
    else if (action == "attack" && sprite->getTexture() != ResourceManager::getTexture(ENEMY_ATTACK_IMAGE) && animationState == CharacterAnimation::ATTACK)
    {
        sprite->setTexture(*ResourceManager::getTexture(ENEMY_ATTACK_IMAGE));
    }

    (*animations)[action]->update(dt, direction);
    sprite->setTextureRect((*animations)[action]->textureRect);
}

void Enemy::move(float deltaTime)
{
    sf::Vector2f movement(0, 0);
    if (animationState == CharacterAnimation::ATTACK && !isAnimationCompleted("attack"))
    {
        updateAnimation("attack", deltaTime);
    }
    else
    {
        animationState = CharacterAnimation::WALK;
        float directionX = targetPosition.x - currentPosition.x;
        float directionY = targetPosition.y - currentPosition.y;
        float magnitude = sqrt(pow(directionX, 2) + pow(directionY, 2));

        if (targetPosition.x < currentPosition.x)
        {
            movement.x -= speed * deltaTime;
        }
        else if (targetPosition.x > currentPosition.x)
        {
            movement.x += speed * deltaTime;
        }

        if (targetPosition.y < currentPosition.y)
        {
            movement.y -= speed * deltaTime;
        }
        else if (targetPosition.y > currentPosition.y)
        {
            movement.y += speed * deltaTime;
        }

        if (movement.x > 0)
            direction = CharacterDirection::RIGHT;
        else
            direction = CharacterDirection::LEFT;

        sprite->move(movement);

        if (magnitude > 0.5f)
        {
            currentPosition.x += (directionX / magnitude) * speed * deltaTime;
            currentPosition.y += (directionY / magnitude) * speed * deltaTime;
            sprite->setPosition(currentPosition);

            updateAnimation("walk", deltaTime);
        }
    }
}

void Enemy::doAttack(sf::Vector2f &target, float dt)
{
    if (animationState == CharacterAnimation::ATTACK || !weapon->isReadyToAttack())
        return;

    (*animations)["attack"]->reset();

    if (target.x >= currentPosition.x)
        direction = CharacterDirection::RIGHT;
    else
        direction = CharacterDirection::LEFT;

    animationState = CharacterAnimation::ATTACK;
    updateAnimation("attack", dt);

    sf::Vector2f current = sf::Vector2f(currentPosition);
    weapon->shoot(target, current, false);
}
