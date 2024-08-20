#include "../interfaces/Hero.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Hero::Hero(float width, float height, float speed, int maxLife, float posX, float posY)
    : Character(width, height, speed, maxLife, posX, posY)
{
    direction = CharacterDirection::RIGHT; // Starts facing right by default
    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();
    weapon = std::make_shared<RangedWeapon>(10, 0.5, 50);
    sprite->setTexture(*ResourceManager::getTexture(HERO_IDLE_IMAGE));
    sprite->setScale(2, 2);
    initAnimations();
};

void Hero::initAnimations()
{
    std::cout << "inside initAnimation\n";
    auto idle = std::make_shared<Animation>(ResourceManager::getTexture(HERO_IDLE_IMAGE), sf::Vector2u(6, 1), 0.08f);
    auto walk = std::make_shared<Animation>(ResourceManager::getTexture(HERO_WALK_IMAGE), sf::Vector2u(8, 1), 0.05f);

    (*animations)["idle"] = idle;
    (*animations)["walk"] = walk;
}

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

    this->weapon->shoot(target, heroPosition,true);
}

void Hero::move(float deltaTime)
{
    sf::Vector2f movement(0, 0);

    // Need this because target is integer and current is float, so comparing precisely these two won't work
    float distanceX = currentPosition.x - targetPosition.x;
    float distanceY = currentPosition.y - targetPosition.y;

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

    if (distanceX < 1 && distanceY < 1 && distanceX > -1 && distanceY > -1)
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

    /// TODO: remove this
    // float currentSpeed = this->speed;
    // if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    // {
    //     currentSpeed += 4;
    // }

    // sf::Vector2f currentPosition = sprite->getPosition();
    // sf::Vector2f direction = this->targetPosition - currentPosition;
    // float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // if (distance < 1.0f)
    //     return;

    // direction /= distance;

    // deltaTime = 0.08f;
    // sprite->move(direction * currentSpeed * deltaTime);

    sf::Vector2f newPosition = sprite->getPosition();
    this->setCurrentPosition(newPosition);
}

void Hero::updateAnimation(const std::string &action, float dt)
{
    // Avoid changing textures so often
    if (action == "idle" && sprite->getTexture() != ResourceManager::getTexture(HERO_IDLE_IMAGE))
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_IDLE_IMAGE));
    }
    else if (action == "walk" && sprite->getTexture() != ResourceManager::getTexture(HERO_WALK_IMAGE))
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_WALK_IMAGE));
    }

    (*animations)[action]->update(dt, direction);

    // Sets texture rect to render updated sprite
    sprite->setTextureRect((*animations)[action]->textureRect);
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