#pragma once
#include "Enemy.hpp"
#include "Hero.hpp"
#include "Base.hpp"
#include "Drop.hpp"
#include "Menu.hpp"
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include "Projectile.hpp"
#include <SFML/Audio.hpp>

/// @brief The central point of all game.
class Game
{
private:
    sf::Clock clock;
    std::unique_ptr<sf::Music> gameovermusic;
    std::unique_ptr<sf::Music> battlemusic;
    float deltaTime;
    float spawnInterval;
    float spawnTimer = 0;
    float enemySpd;
    int enemyLife;
    int enemyDamage;

    /// @brief A varieable that show the count os killf the player have in the game
    int killCounter = 0;

    float gameTime;
    float healTime = 12.0f;

    std::unique_ptr<Menu> menu;

    GameDifficulty difficulty;

    /// @brief Resolve conflicts with projectiles and characters (enemies or hero), erasing projectiles if it need to
    /// @tparam T A class derived from Character or Base
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
                // If projectile isn't colliding with enemy, or if it is but enemy is dead, projectile keeps on screen.

                auto character = *characterIt;
                if (!character->isCollidingWith(projectile->getBounds()))
                    continue;

                if constexpr (std::is_same<Enemy, T>::value)
                {
                    if (character->isDead())
                        continue;
                }

                projectileIt = projectiles->erase(projectileIt);

                character->takeDamage(projectile->getDamage());

                if constexpr (std::is_same<Enemy, T>::value)
                {
                    if (character->isDead())
                    {
                        killCounter++;

                        if (character->hasDrop())
                        {
                            sf::Vector2f pos = character->getCurrentPosition();
                            spawnDrop(pos);
                        }
                    }
                }
            }
            projectile->update(deltaTime);
        }
    }

    /// @brief Changes to game over screen
    void showGameOver();
    sf::Vector2f getMousePosition();
    void setDeltaTime(float time);
    /// @brief Changes to game win screen
    void showGameWin();
    void updateBase();
    void updateHero();
    void updateEnemies();
    void updateDrops();
    void dealCollisions();

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
    int addDefense;
    /// @brief An unique pointer to background sprite
    std::unique_ptr<sf::Sprite> background;
    /// @brief A pointer to game's window
    std::shared_ptr<sf::RenderWindow> gameWindow;
    /// @brief Render some information on screen (life and ammo).
    void renderStatus();
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
    void update();
    /// @brief Renders the actual state of objects on screen.
    void render();
    /// @brief Closes the window and ends the game.
    void close();

public:
    Game(float x, float y, std::shared_ptr<sf::RenderWindow> window, GameDifficulty difficulty);

    void setDifficulty(GameDifficulty diff);

    /// @brief Start point to run game.
    void run();
};
