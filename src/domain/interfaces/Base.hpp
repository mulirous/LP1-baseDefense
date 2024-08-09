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
    sf::Sprite baseSprite;
    sf::Texture baseTexture;

public:
    Base(float radius, int maxLife, int currentLife, float posX, float posY)
        : maxLife(maxLife), currentLife(currentLife), currentPosition(posX, posY)
    {
        if (!baseTexture.loadFromFile(BASE_IMAGE))
        {
            throw std::runtime_error("Failed to load base texture");
        }
        baseSprite.setTexture(baseTexture);

        // Escala a sprite para reduzir o tamanho
        float scaleFactor = 0.6f; // diminui para 60% do tamanho original
        baseSprite.setScale(scaleFactor, scaleFactor);

        baseSprite.setPosition(posX - (baseSprite.getGlobalBounds().width / 2),
                               posY - (baseSprite.getGlobalBounds().height / 2));
    }
    sf::Sprite getSprite() { return this->baseSprite; }
    sf::Vector2f getPosition() { return this->currentPosition; }

    void takeDamage(int damage)
    {
        if (damage <= 0)
            return;
        this->currentLife -= damage;
    }

    int getLife() const { return this->currentLife; }
    bool isCollidingWith(const sf::FloatRect &other) const
    {
        return baseSprite.getGlobalBounds().intersects(other);
    }
};

#endif
