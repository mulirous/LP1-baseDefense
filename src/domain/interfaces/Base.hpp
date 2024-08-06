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

    sf::Vector2f getPosition() { return this->currentPosition; }

    void takeDamage(int damage) {
        if (damage <= 0)
            return;
        this->currentLife -= damage;
    }

    int getLife() const { return this->currentLife; }

    bool isCollidingWith(const sf::FloatRect &other) const {
        return shape.getGlobalBounds().intersects(other);
    }
};

#endif