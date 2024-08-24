#pragma once

#include "Enemy.hpp"
#include "Hero.hpp"
#include "Base.hpp"
#include "Drop.hpp"
#include "Menu.hpp"
#include "../enums/GameState.h"
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>
#include "Projectile.hpp"
#include <SFML/Audio.hpp>

/// @file Game.hpp
/// @class Game
/// @brief The central point of all game operations and state management.
///
/// The Game class is responsible for managing the game's core functionalities, including
/// the game loop, state transitions, object updates, rendering, and event handling.
/// It also manages the game difficulty, spawns enemies and drops, and handles game states such as win and game over.
///
/// The Game class relies on SFML for graphics and audio handling, and uses various
/// game-specific elements such as enemies, the hero, drops, and a base.
///
/// @note The entire game (not only Game object) uses smart pointers for resource management and avoids raw pointers
/// to ensure proper memory handling and avoid leaks.
class Game
{
private:
    /// @brief A clock used to measure time elapsed between frames.
    sf::Clock clock;

    /// @brief  Music played during game over.
    std::unique_ptr<sf::Music> gameovermusic;

    /// @brief Music played during gameplay.
    std::unique_ptr<sf::Music> battlemusic;

    /// @brief Time elapsed between frames.
    float deltaTime;

    /// @brief Interval between enemy spawns.
    /// @note Set when difficulty is chosen.
    float spawnInterval;

    /// @brief Timer to track the next enemy spawn.
    /// @note Set when difficulty is chosen.
    float spawnTimer = 0;

    /// @brief Health of enemies.
    /// @note Set when difficulty is chosen.
    int enemyLife;

    /// @brief Damage dealt by enemies.
    /// @note Set when difficulty is chosen.
    int enemyDamage;

    /// @brief A variable that shows the count of kills the player has in the game.
    int killCounter = 0;

    /// @brief Remaining game time.
    /// @note Set when difficulty is chosen.
    float gameTime;

    /// @brief A unique pointer to the game's menu.
    std::unique_ptr<Menu> menu;

    /// @brief Current difficulty level of the game.
    /// @details It changes some parameters such as enemies damage, game time and enemies life.
    GameDifficulty difficulty;

    /// @brief Current state of the game (e.g., MENU, PLAY, EXIT).
    GameState state;

    /// @brief Resolves collisions between projectiles and characters (enemies or hero), removing projectiles as needed.
    /// @details This method iterates through the list of projectiles and characters, checking for collisions.
    /// If a collision is detected, the projectile is removed, and the character takes damage.
    /// If the character is an enemy and is killed, a kill counter is incremented, and a drop may be spawned.
    /// @tparam T A class derived from Character or Base.
    /// @param projectiles A shared pointer to a list of projectile pointers.
    /// @param characters A shared pointer to a list of T pointers.
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

    /// @brief Retrieves the current mouse position relative to the game window.
    /// @return The current mouse position as an `sf::Vector2f`.
    sf::Vector2f getMousePosition();

    /// @brief Sets the delta time between frames.
    /// @param time The delta time in seconds.
    void setDeltaTime(float time);

    /// @brief Updates the base object's state.
    void updateBase();

    /// @brief Updates the hero's state.
    void updateHero();

    /// @brief Updates the enemies' states.
    void updateEnemies();

    /// @brief Updates the drops' states.
    void updateDrops();

    /// @brief Handles collisions between game objects.
    void dealCollisions();

    /// @brief Initializes/resets game objects.
    void initializeObjects();

    /// @brief Resets game parameters and clears objects.
    void restart();

protected:
    /// @brief The x-coordinate of the screen's center.
    float centerX;

    /// @brief The y-coordinate of the screen's center.
    float centerY;

    /// @brief A shared pointer to a list of enemy pointers.
    std::shared_ptr<std::list<std::shared_ptr<Enemy>>> enemies;

    /// @brief A unique pointer to a list of drop pointers.
    std::unique_ptr<std::list<std::shared_ptr<Drop>>> drops;

    /// @brief A shared pointer to the hero object.
    std::shared_ptr<Hero> hero;

    /// @brief A shared pointer to the base object.
    std::shared_ptr<Base> base;

    /// @brief A unique pointer to the background sprite.
    std::unique_ptr<sf::Sprite> background;

    /// @brief A shared pointer to the game's render window.
    std::shared_ptr<sf::RenderWindow> gameWindow;

    /// @brief Renders game status information on the screen, such as life, ammo, base life, kills, and time.
    void renderStatus();

    /// @brief Adds a new enemy to the game and places it at a random position on the screen.
    /// @note Internally adds the enemy to the list of enemies.
    void spawnEnemy();

    /// @brief Adds a new drop to the game at the specified position.
    /// @param enemyPosition The position where the drop will spawn.
    /// @note Internally adds the drop to the list of drops.
    void spawnDrop(sf::Vector2f &enemyPosition);

    /// @brief Processes user input and other events, such as closing the window or initiating attacks.
    void handleEvents();

    /// @brief Updates the game state, including objects and their interactions.
    void update();

    /// @brief Renders the current state of all game objects on the screen.
    void render();

    /// @brief Closes the game window and ends the game.
    void close();

    /// @brief Renders the ending screen, showing whether the game was won or lost.
    /// @param isSuccess Indicates whether the game was won (true) or lost (false).
    void renderEnding(bool isSuccess);

    /// @brief The main game loop, managing game execution and state transitions.
    void run();

public:
    /// @brief Constructs Game with the specified render window.
    /// @param window A shared pointer to the game's render window.
    Game(std::shared_ptr<sf::RenderWindow> window);

    /// @brief Starts the game by initializing objects and entering the game loop.
    void start();
};