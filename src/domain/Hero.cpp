#include "interfaces/Hero.hpp"
#include "interfaces/Game.hpp"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cmath>

void Hero::move(float deltaTime)
{
    sf::Clock clock;
    deltaTime = clock.restart().asSeconds() + 0.5;

    float currentSpeed = this->speed;

    // Sprint on L or R Shift
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
    {
        currentSpeed += 3;
    }

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->positionX > 0)
    {
        moveX -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->positionX + this->width < GameWindowWidth)
    {
        moveX += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->positionY > 0)
    {
        moveY -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->positionY + this->height < GameWindowHeight)
    {
        moveY += 1.0f;
    }

    float diagonal = std::sqrt(moveX * moveX + moveY * moveY);
    if (diagonal != 0)
    {
        moveX /= diagonal;
        moveY /= diagonal;
    }

    this->positionX += moveX * currentSpeed * deltaTime;
    this->positionY += moveY * currentSpeed * deltaTime;

    shape.setPosition(this->positionX, this->positionY);
}