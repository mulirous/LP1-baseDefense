#include "../interfaces/Character.hpp"
#include <SFML/Graphics.hpp>

Character::Character(float width, float height, float speed, int maxLife, float posX, float posY) : width(width), height(height), speed(speed), animationState(CharacterAnimation::IDLE),
                                                                                                    maximumLife(maxLife), currentLife(maxLife), currentPosition(posX, posY)
{
    sprite = std::make_shared<sf::Sprite>();
    sprite->setPosition(posX, posY);
};

Character::Character(float width, float height, float speed, int maxLife, sf::Vector2f position) : Character(width, height, speed, maxLife, position.x, position.y) {};

float Character::getWidth()
{
    return this->width;
}

float Character::getHeigth()
{
    return this->height;
}

float Character::getSpeed()
{
    return this->speed;
}

int Character::getMaxLife()
{
    return this->maximumLife;
}

int Character::getLife()
{
    return this->currentLife;
}

sf::Vector2f Character::getCurrentPosition()
{
    return this->currentPosition;
}

sf::FloatRect Character::getGlobalBounds()
{
    return sf::FloatRect(this->currentPosition.x, this->currentPosition.y, this->width, this->height);
}

CharacterAnimation Character::getAnimationState() const
{
    return this->animationState;
}

void Character::setAnimationState(CharacterAnimation &animation)
{
    this->animationState = animation;
}

std::shared_ptr<sf::Sprite> Character::getSprite()
{
    return this->sprite;
}

void Character::setCurrentPosition(sf::Vector2f &position)
{
    this->currentPosition = position;
}

bool Character::isCollidingWith(const sf::FloatRect &rect)
{
    return this->getGlobalBounds().intersects(rect);
}

bool Character::isCollidingWith(std::shared_ptr<Character> other)
{
    return this->getGlobalBounds().intersects(other->getGlobalBounds());
}

bool Character::isAnimationCompleted(const std::string &action)
{
    return (*animations)[action]->isCompleted();
}

void Character::resolveCollision(std::shared_ptr<Character> other)
{
    float overlapX = (this->width / 2 + other->width / 2) - std::abs(this->currentPosition.x - other->currentPosition.x);
    float overlapY = (this->height / 2 + other->height / 2) - std::abs(this->currentPosition.y - other->currentPosition.y);

    if (overlapX < overlapY)
    {
        if (this->currentPosition.x < other->currentPosition.x)
            this->currentPosition.x -= overlapX / 2;
        else
            this->currentPosition.x += overlapX / 2;

        if (this->currentPosition.x < other->currentPosition.x)
            other->currentPosition.x += overlapX / 2;
        else
            other->currentPosition.x -= overlapX / 2;
    }
    else
    {
        if (this->currentPosition.y < other->currentPosition.y)
            this->currentPosition.y -= overlapY / 2;
        else
            this->currentPosition.y += overlapY / 2;

        if (this->currentPosition.y < other->currentPosition.y)
            other->currentPosition.y += overlapY / 2;
        else
            other->currentPosition.y -= overlapY / 2;
    }
}