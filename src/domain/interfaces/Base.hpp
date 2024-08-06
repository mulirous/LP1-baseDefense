#ifndef BASE_HPP
#define BASE_HPP
#include <SFML/Graphics.hpp>
#include <memory>

class Base
{
private:
    int maxLife;
    int currentLife;
    sf::Vector2f currentPosition;
    sf::CircleShape shape;

public:
    Base(float radius, int maxLife, int currentLife, float posX, float posY) :
    maxLife(maxLife), currentLife(currentLife), currentPosition(posX, posY)
    {
        shape.setRadius(radius);
        shape.setOutlineThickness(5); 
        shape.setOutlineColor(sf::Color::Blue);
        shape.setFillColor(sf::Color::Transparent);
        shape.setPosition(posX - radius, posY - radius);
    }

    sf::CircleShape getShape() { return this->shape; }

};

#endif