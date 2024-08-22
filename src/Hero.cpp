#include "../interfaces/Hero.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

Hero::Hero(float width, float height, float speed, int maxLife, float posX, float posY)
    : Character(width, height, speed, maxLife, posX, posY)
{
    direction = CharacterDirection::RIGHT; // Starts facing right by default
    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();
    weapon = std::make_shared<RangedWeapon>(10, 0.5, 50, 25);
    sprite->setTexture(*ResourceManager::getTexture(HERO_IDLE_IMAGE));
    sprite->setScale(2, 2);
    initAnimations();
};

void Hero::initAnimations()
{
    // Each element represents a different frame from attack spritesheet
    auto attackFrames = std::make_unique<std::vector<sf::IntRect>>(std::initializer_list<sf::IntRect>({sf::IntRect(0, 0, 17, 26),
                                                                                                       sf::IntRect(17, 0, 18, 26),
                                                                                                       sf::IntRect(35, 0, 17, 26),
                                                                                                       sf::IntRect(52, 0, 19, 26),
                                                                                                       sf::IntRect(71, 0, 28, 26),
                                                                                                       sf::IntRect(99, 0, 28, 26)}));

    auto idle = std::make_shared<Animation>(ResourceManager::getTexture(HERO_IDLE_IMAGE), sf::Vector2u(6, 1), 0.08f);
    auto walk = std::make_shared<Animation>(ResourceManager::getTexture(HERO_WALK_IMAGE), sf::Vector2u(8, 1), 0.05f);
    auto attack = std::make_shared<Animation>(ResourceManager::getTexture(HERO_ATTACK_IMAGE), sf::Vector2u(6, 1), 0.05f, std::move(attackFrames));

    (*animations)["idle"] = idle;
    (*animations)["walk"] = walk;
    (*animations)["attack"] = attack;
}

void Hero::takeDamage(int damage)
{
    if (damage <= 0)
        return;
    this->currentLife -= damage;
}

void Hero::doAttack(sf::Vector2f &target, float dt)
{
    // Hero can't attack again if it's still on animation
    if (animationState == CharacterAnimation::ATTACK || !weapon->isReadyToAttack())
        return;

    (*animations)["attack"]->reset();

    // Get hero position
    auto heroPosition = sf::Vector2f(this->currentPosition);

    // Sets hero direction
    if (target.x >= heroPosition.x)
        direction = CharacterDirection::RIGHT;
    else
        direction = CharacterDirection::LEFT;

    animationState = CharacterAnimation::ATTACK;
    updateAnimation("attack", dt);

    weapon->shoot(target, heroPosition, true);
}

void Hero::move(float deltaTime)
{
    sf::Vector2f movement(0, 0);
    if (animationState == CharacterAnimation::ATTACK && !isAnimationCompleted("attack"))
    {
        updateAnimation("attack", deltaTime);
    }
    else
    {
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
            // Hero is on target destination (or closely enough)
            animationState = CharacterAnimation::IDLE;
            updateAnimation("idle", deltaTime);
        }
        else
        {
            // Hero isn't close to target, so it will move
            if (movement.x > 0)
                this->direction = CharacterDirection::RIGHT;
            else
                this->direction = CharacterDirection::LEFT;

            animationState = CharacterAnimation::WALK;
            updateAnimation("walk", deltaTime);
        }
    }
    sprite->move(movement);

    sf::Vector2f newPosition = sprite->getPosition();
    setCurrentPosition(newPosition);
}

void Hero::updateAnimation(const std::string &action, float dt)
{
    if (action == "idle" && sprite->getTexture() != ResourceManager::getTexture(HERO_IDLE_IMAGE) && animationState == CharacterAnimation::IDLE)
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_IDLE_IMAGE));
    }
    else if (action == "walk" && sprite->getTexture() != ResourceManager::getTexture(HERO_WALK_IMAGE) && animationState == CharacterAnimation::WALK)
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_WALK_IMAGE));
    }
    else if (action == "attack" && sprite->getTexture() != ResourceManager::getTexture(HERO_ATTACK_IMAGE) && animationState == CharacterAnimation::ATTACK)
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_ATTACK_IMAGE));
    }

    (*animations)[action]->update(dt, direction);

    // Sets texture rect to render updated sprite
    sprite->setTextureRect((*animations)[action]->textureRect);
}

void Hero::heal(int healAmount)
{
    if (healAmount + currentLife > maximumLife)
    {
        this->currentLife = maximumLife;
        return;
    }

    this->currentLife += healAmount;
}

void Hero::recharge(int ammo)
{
    this->weapon->addAmmo(ammo);
};

void Hero::setTargetPosition(sf::Vector2f target)
{
    this->targetPosition = target;
}