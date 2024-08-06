#include "interfaces/Character.hpp"
#include <SFML/Graphics.hpp>

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