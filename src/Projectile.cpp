#include "../interfaces/Projectile.hpp"

Projectile::Projectile(int damage, float velocity, const sf::Vector2f &position, const sf::Vector2f &target)
    : position(position), damage(damage), velocity(velocity), target(target)
{
    shape.setRadius(5);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(this->position);
    shape.setRotation(atan2(target.y, target.x) * 180 / 3.14159265);
}

void Projectile::update(float deltaTime)
{
    shape.move(target * velocity * deltaTime);
}

bool Projectile::isOffScreen() const
{
    sf::FloatRect bounds = shape.getGlobalBounds();
    return (bounds.left + bounds.width < 0 || bounds.left > GAME_WINDOW_WIDTH ||
            bounds.top + bounds.height < 0 || bounds.top > GAME_WINDOW_HEIGHT);
}