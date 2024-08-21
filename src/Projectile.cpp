#include "../interfaces/Projectile.hpp"
#include "SFML/Graphics.hpp"


Projectile::Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target, bool isHero)
    : damage(damage), velocity(velocity), position(position), target(target)
{
    sprite = std::make_shared<sf::Sprite>();
    sprite->setPosition(position);

    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();

    if (isHero) {
        sprite->setTexture(*ResourceManager::getTexture(WIZARD_SPELL));
    } else {
        sprite->setTexture(*ResourceManager::getTexture(ENEMY_ARROW));
    }

    initAnimations();
}


void Projectile::initAnimations()
{
    // Initialize the projectile animation
    auto attack = std::make_shared<Animation>(ResourceManager::getTexture(WIZARD_SPELL), sf::Vector2u(8, 1), 0.05f);
    (*animations)["attack"] = attack;
}

const sf::Sprite &Projectile::getSprite()
{
    return *sprite;
}

sf::FloatRect Projectile::getBounds()
{
    return sprite->getGlobalBounds();
}

int Projectile::getDamage()
{
    return damage;
}

void Projectile::update(float deltaTime)
{
    // Update the animation
    (*animations)["attack"]->update(deltaTime);
    sprite->setTextureRect((*animations)["attack"]->textureRect);

    // Move the projectile
    sf::Vector2f direction = target - position;
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0.0f)
        direction /= magnitude;

    position += direction * velocity * deltaTime;
    sprite->setPosition(position);
}

bool Projectile::isOffScreen() const
{
    // Check if the projectile is off screen
    return position.x < 0 || position.x > GAME_WINDOW_WIDTH || position.y < 0 || position.y > GAME_WINDOW_HEIGHT;
}
