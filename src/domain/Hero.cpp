#include "interfaces/Hero.hpp"
#include "interfaces/Game.hpp"
#include <SFML/Graphics.hpp>

void Hero::move()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->positionX > 0)
    {
        this->positionX -= this->speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->positionX + this->width < Game::windowWidth)
    {
        this->positionX += this->speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->positionY > 0)
    {
        this->positionY -= this->speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->positionY + this->height < Game::windowHeight)
    {
        this->positionY += this->speed;
    }
    shape.setPosition(this->positionX, this->positionY);
}