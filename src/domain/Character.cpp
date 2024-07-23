#include "interfaces/Character.hpp"
#include <SFML/Graphics.hpp>

Character::Character(float width, float height, float speed, int maxLife, float posX, float posY)
{
    this->width = width;
    this->height = height;
    this->speed = speed;
    this->maximumLife = maxLife;
    this->currentLife = maxLife;
    this->positionX = posX;
    this->positionY = posY;
};

Character::~Character()
{
    delete this;
}

void Character::setShapePosition(float x, float y)
{
    this->positionX = x;
    this->positionY = y;
    shape.setPosition(x, y);
}