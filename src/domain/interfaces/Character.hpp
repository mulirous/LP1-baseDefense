#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <SFML/Graphics.hpp>

/// @brief An abstract class to serve as base to Hero and Enemy
class Character
{
protected:
    /// @brief Width on pixels
    float width;
    /// @brief Height on pixels
    float height;
    /// @brief How many pixels character moves in a single movement
    float speed;
    /// @brief Max life of character
    int maximumLife;
    /// @brief Current life of character
    int currentLife;
    /// @brief Object representing the shape of character
    // TO-DO: change this shape
    sf::RectangleShape shape;
    /// @brief X-axis position of character on screen
    float positionX;
    /// @brief Y-axis position of character on screen
    float positionY;

public:
    Character(float width, float height, float speed, int maxLife, float posX, float posY);
    ~Character();
    float getWidth() { return width; }
    float getHeigth() { return height; }
    float getSpeed() { return speed; }
    int getMaxLife() { return maximumLife; }
    int getLife() { return currentLife; }
    float getPosX() { return positionX; }
    float getPosY() { return positionY; }
    sf::RectangleShape getShape() { return shape; }
    void setShapePosition(float x, float y);
    virtual void move(float windowWidth, float windowHeight) = 0; // Marked as "virtual" to be overriden by its child-classes (hero, enemy)
};

#endif