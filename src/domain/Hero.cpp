#include "interfaces/Hero.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

void Hero::move()
{
    float currentSpeed = this->speed;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        currentSpeed += 3;
    }

    sf::Vector2f currentPosition = shape.getPosition();
    sf::Vector2f direction = this->targetPosition - currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f)
    {
        return;
    }

    direction /= distance;

    float deltaTime = 0.1f; 
    shape.move(direction * currentSpeed * deltaTime);

    sf::Vector2f newPosition = shape.getPosition();
    this->positionX = newPosition.x;
    this->positionY = newPosition.y;
}
