#include "../interfaces/Projectile.hpp"
#include "SFML/Graphics.hpp"
#include <cmath>
#define _USE_MATH_DEFINES

Projectile::Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target, bool isHero)
    : damage(damage), velocity(velocity), position(position), target(target)
{
    sprite = std::make_shared<sf::Sprite>();
    sprite->setPosition(position);

    if (isHero) {
        sprite->setTexture(*ResourceManager::getTexture(WIZARD_SPELL));
        initAnimations();  // Apenas o projétil do herói tem animação
    } else {
        sprite->setTexture(*ResourceManager::getTexture(ENEMY_ARROW));
        sprite->setScale(2.0f, 2.0f);  // Aumenta o tamanho da sprite do inimigo

        // Calcular o ângulo de rotação para o projétil do inimigo
        sf::Vector2f direction = target - position;
        float angle = atan2(direction.y, direction.x) * 180 / 3.14;
        sprite->setRotation(angle);  // Agora ajusta para que a seta aponte na direção correta
    }
}


void Projectile::initAnimations()
{
    // Inicializa a animação do projétil do herói
    auto attack = std::make_shared<Animation>(const_cast<sf::Texture*>(sprite->getTexture()), sf::Vector2u(8, 1), 0.05f);
    animations = std::make_shared<std::map<std::string, std::shared_ptr<Animation>>>();
    (*animations)["attack"] = attack;
}

void Projectile::update(float deltaTime)
{
    if (animations) {
        // Atualiza a animação apenas se existir
        (*animations)["attack"]->update(deltaTime);
        sprite->setTextureRect((*animations)["attack"]->textureRect);
    }

    // Movimenta o projétil na direção do alvo
    sf::Vector2f direction = target - position;
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0.0f)
        direction /= magnitude;

    position += direction * velocity * deltaTime;
    sprite->setPosition(position);
}

const sf::Sprite &Projectile::getSprite()
{
    return *sprite;
}

sf::FloatRect Projectile::getBounds()
{
    return sprite->getGlobalBounds();
}

int Projectile::getDamage()
{
    return this->damage;
}

bool Projectile::isOffScreen() const
{
     sf::FloatRect bounds = sprite->getGlobalBounds();
    return position.x < 0 || position.x > GAME_WINDOW_WIDTH || position.y < 0 || position.y > GAME_WINDOW_HEIGHT;
}
