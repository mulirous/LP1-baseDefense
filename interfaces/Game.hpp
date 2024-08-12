#pragma once
#include "Enemy.hpp"
#include "Hero.hpp"
#include "Base.hpp"
#include "Drop.hpp"
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include "Projectile.hpp"
#include "../src/modules/animation/src/AnimationManager.hpp"

/// @brief The central point of all game.
class Game
{
private:
    /// @brief Resolve conflicts with projectiles and characters (enemies or hero), erasing projectiles if it need to
    /// @tparam T A class derived from Character
    /// @param projectiles A shared pointer to a list of Projectiles pointers
    /// @param characters A shared pointer to a list of T pointers
    template <typename T>
    void calculateCollisionsWithProjectiles(
        std::shared_ptr<std::list<std::shared_ptr<Projectile>>> projectiles,
        std::shared_ptr<std::list<std::shared_ptr<T>>> characters)
    {
        static_assert(std::is_base_of<Character, T>::value || std::is_same<Base, T>::value, "T must be derived from Character or Base!");

        for (auto projectileIt = projectiles->begin(); projectileIt != projectiles->end(); projectileIt++)
        {
            auto projectile = *projectileIt;
            if (projectile->isOffScreen())
            {
                projectileIt = projectiles->erase(projectileIt);
                continue;
            }
            for (auto characterIt = characters->begin(); characterIt != characters->end(); characterIt++)
            {
                auto character = *characterIt;
                if (!character->isCollidingWith(projectile->getBounds()))
                    continue;

                projectileIt = projectiles->erase(projectileIt);
                if constexpr (std::is_same<Hero, T>::value || std::is_same<Base, T>::value)
                {
                    character->takeDamage(projectile->getDamage());
                }
                else if constexpr (std::is_same<Enemy, T>::value)
                {
                    character->kill();
                    if (character->hasDrop())
                    {
                        sf::Vector2f pos = character->getCurrentPosition();
                        spawnDrop(pos);
                    }
                }
            }
            projectile->update(deltaTime);
        }
    }

    /// @brief Changes to game over screen
    void showGameOver();

protected:
    /// @brief The screen's center on x-axis
    float centerX;
    /// @brief The screen's center on y-axis
    float centerY;
    /// @brief A pointer to a list of enemies pointers
    std::shared_ptr<std::list<std::shared_ptr<Enemy>>> enemies;
    std::unique_ptr<std::list<std::shared_ptr<Drop>>> drops;
    /// @brief A pointer to the hero
    std::shared_ptr<Hero> hero;
    /// @brief A pointer to the base
    std::shared_ptr<Base> base;
    /// @brief A pointer to game's window
    std::shared_ptr<sf::RenderWindow> gameWindow;
    std::shared_ptr<AnimationManager> animationManager;
    float deltaTime;
    float spawnInterval = 5;
    float spawnTimer = 0;
    /// @brief Render some information on screen (life and ammo).
    void renderStatus();
    /// @brief Textures for hero, enemy, and base
    sf::Texture heroTexture;
    sf::Texture enemyTexture;
    sf::Texture baseTexture;

    /// @brief Sprites for hero, enemy, and base
    sf::Sprite heroSprite;
    sf::Sprite enemySprite;
    sf::Sprite baseSprite;

    /// @brief Adds a new enemy to game
    /// @note Internally, it adds a enemy to enemies's list
    void spawnEnemy();
    /// @brief Adds a new drop to game
    /// @param enemyPosition Position on drop will spawn
    /// @note Internally, adds drop to drops's list
    void spawnDrop(sf::Vector2f &enemyPosition);

    /// @brief Process events like inputs.
    void handleEvents();
    /// @brief Changes the state of objects.
    /// @param time
    void update(float time);
    /// @brief Renders the actual state of objects on screen.
    void render();
    /// @brief Closes the window and ends the game.
    void close();

public:
    Game(float x, float y, std::shared_ptr<sf::RenderWindow> window);
    sf::Vector2f getMousePosition() { return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->gameWindow)); };
    void setDeltaTime(float time) { this->deltaTime = time; }
    void setHero(std::shared_ptr<Hero> hero) { this->hero = hero; }
    void setBase(std::shared_ptr<Base> base) { this->base = base; }

    /// @brief Start point to run game.
    void run();
};
