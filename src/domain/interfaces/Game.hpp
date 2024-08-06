#ifndef GAME_HPP
#define GAME_HPP
#include "Enemy.hpp"
#include "Hero.hpp"
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include "Projectile.hpp"

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
        // Verify if type is derived from Character or not
        static_assert(std::is_base_of<Character, T>::value, "T must be derived from Character!");

        for (auto projectileIt = projectiles->begin(); projectileIt != projectiles->end(); projectileIt++)
        {
            auto projectile = *projectileIt;
            if (projectile->isOffScreen())
            {
                projectileIt = projectiles->erase(projectileIt);
                continue;
            }
            for (auto characterIt = characters->begin(); characterIt != characters->end();)
            {
                auto character = *characterIt;
                if (character->isCollidingWith(projectile->getBounds()))
                {
                    projectileIt = projectiles->erase(projectileIt);
                    if constexpr (std::is_same<Hero, T>::value)
                    {
                        character->takeDamage(projectile->getDamage());
                        if (character->getLife() <= 0)
                        {
                            // GAME OVER
                        }
                    }
                    else if constexpr (std::is_same<Enemy, T>::value)
                    {
                        characterIt = characters->erase(characterIt);
                    }
                    break;
                }
                else
                {
                    ++characterIt;
                }
            }
            projectile->update(deltaTime);
        }
    }

protected:
    /// @brief The screen's center on x-axis
    float centerX;
    /// @brief The screen's center on y-axis
    float centerY;
    /// @brief A unique pointer to a list of enemies pointers
    std::shared_ptr<std::list<std::shared_ptr<Enemy>>> enemies;
    /// @brief A pointer to the hero
    std::shared_ptr<Hero> hero;
    /// @brief A unique pointer to game's window
    std::unique_ptr<sf::RenderWindow> gameWindow;
    float deltaTime;
    float spawnInterval = 2;
    float spawnTimer = 0;
    /// @brief Render some information on screen (life and ammo).
    void renderStatus();

public:
    Game(float x, float y) : centerX(x), centerY(y),
                             enemies(std::make_shared<std::list<std::shared_ptr<Enemy>>>()),
                             gameWindow(std::make_unique<sf::RenderWindow>(sf::VideoMode(1200, 800), "Game Window"))
    {
        srand(static_cast<unsigned>(time(0)));
    };
    sf::Vector2f getMousePosition() { return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->gameWindow)); };
    void setDeltaTime(float time) { this->deltaTime = time; }
    void setHero(std::shared_ptr<Hero> hero) { this->hero = hero; }

    /// @brief Start point to run game.
    void run();
    /// @brief Creates new enemy
    /// @return Pointer to enemy
    std::shared_ptr<Enemy> spawnEnemy();
    /// @brief Process events like inputs.
    void handleEvents();
    /// @brief Changes the state of objects.
    /// @param time
    void update(float time);
    /// @brief Renders the actual state of objects on screen.
    void render();
    /// @brief Closes the window and ends the game.
    void close();
};

#endif