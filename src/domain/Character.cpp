#include "interfaces/Character.hpp"
#include <SFML/Graphics.hpp>

void Character::resolveCollision(std::shared_ptr<Character> other)
{
    float overlapX = (this->width / 2 + other->width / 2) - std::abs(this->positionX - other->positionX);
    float overlapY = (this->height / 2 + other->height / 2) - std::abs(this->positionY - other->positionY);

    if (overlapX < overlapY)
    {
        if (this->positionX < other->positionX)
            this->positionX -= overlapX / 2;
        else
            this->positionX += overlapX / 2;

        if (this->positionX < other->positionX)
            other->positionX += overlapX / 2;
        else
            other->positionX -= overlapX / 2;
    }
    else
    {
        if (this->positionY < other->positionY)
            this->positionY -= overlapY / 2;
        else
            this->positionY += overlapY / 2;

        if (this->positionY < other->positionY)
            other->positionY += overlapY / 2;
        else
            other->positionY -= overlapY / 2;
    }
}