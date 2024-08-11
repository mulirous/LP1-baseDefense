#include "interfaces/Hero.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Hero::Hero(float width, float height, float speed, int maxLife, float posX, float posY)
    : Character(width, height, speed, maxLife, posX, posY)
{
    currentFrame = 0;
    frameTime = 0.1f;
    timeSinceLastFrame = 0;
    texture.loadFromFile(HERO_IMAGE);
    sprite.setTexture(texture);
    frameSize = sf::Vector2i(width, height);
    sprite.setTextureRect(sf::IntRect(0, 0, frameSize.x, frameSize.y));
    sprite.setPosition(posX, posY);
    weapon = std::make_shared<RangedWeapon>(10, 0.5, 50);

    sprite.setScale(1.5f, 1.5f);
};

void Hero::takeDamage(int damage)
{
    if (damage <= 0)
        return;
    this->currentLife -= damage;
}

void Hero::doAttack(sf::Vector2f &target)
{
    // Get hero position
    auto heroPosition = sf::Vector2f(this->currentPosition);

    // Set mouse position as target on ranged weapon and do attack
    this->weapon->shoot(target, heroPosition);
}

void Hero::move(float deltaTime)
{
    float currentSpeed = this->speed;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        currentSpeed += 4;
    }

    sf::Vector2f currentPosition = sprite.getPosition();
    sf::Vector2f direction = this->targetPosition - currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f)
        return;

    direction /= distance;

    deltaTime = 0.08f;
    sprite.move(direction * currentSpeed * deltaTime);

    sf::Vector2f newPosition = sprite.getPosition();
    this->setCurrentPosition(newPosition);
    updateAnimation(deltaTime);
}

sf::Vector2f Hero::getPosition()
{
    return sprite.getPosition();
}

void Hero::updateAnimation(float deltaTime)
{
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= frameTime)
    {
        timeSinceLastFrame = 0.0f;
        currentFrame = (currentFrame + 1) % (texture.getSize().x / frameSize.x);
        sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, 0, frameSize.x, frameSize.y));
    }
}

void Hero::heal(int healAmount)
{
    if (healAmount + currentLife > 100)
        return;

    this->currentLife += healAmount;
}

void Hero::recharge(int ammo)
{
    this->weapon->addAmmo(ammo);
};

std::shared_ptr<RangedWeapon> Hero::getRangedWeapon()
{
    return this->weapon;
}

void Hero::setTargetPosition(sf::Vector2f target)
{
    this->targetPosition = target;
}