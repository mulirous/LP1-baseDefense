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
    sprite->setScale(3.f, 3.f);
    initAnimations();
};

void Hero::initAnimations()
{
    std::cout << "inside initAnimation\n";
    auto idle = std::make_shared<Animation>(ResourceManager::getTexture(HERO_IDLE_IMAGE), sf::Vector2u(6, 1), 0.05f);
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

    this->weapon->shoot(target, heroPosition);
}

void Hero::move(float deltaTime)
{
    // Esses calculos estão seguindo exatamente o padrão do tutorial, por isso comentei o calculo antigo.
    sf::Vector2f movement(0, 0);

    if (targetPosition.x < currentPosition.x)
    {
        std::cout << "target is on left\n";
        movement.x -= this->speed * deltaTime;
    }
    else
    {
        std::cout << "target is on the right\n";
        movement.x += this->speed * deltaTime;
    }
    if (movement.x == 0)
    {
        std::cout << "idle animation\n";
        updateAnimation("idle", deltaTime);
    }
    else
    {
        std::cout << "walk animation\n";
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
    std::cout << "inside updateAnimation\n";
    std::cout << "direction: " << (int)direction << "\n";

    // Avoid changing textures so often
    if (action == "idle" && sprite->getTexture() != ResourceManager::getTexture(HERO_IDLE_IMAGE))
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_IDLE_IMAGE));
        std::cout << "idle action on updateAnimation\n";
    }
    else if (action == "walk" && sprite->getTexture() != ResourceManager::getTexture(HERO_WALK_IMAGE))
    {
        sprite->setTexture(*ResourceManager::getTexture(HERO_WALK_IMAGE));
        std::cout << "walk action on updateAnimation\n";
    }
    std::cout << "will update animation\n";
    (*animations)[action]->update(dt, direction);
    // Precisa setar a textureRect para renderizar o proximo sprite (após o update)
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