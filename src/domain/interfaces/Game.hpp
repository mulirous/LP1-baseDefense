#ifndef GAME_HPP
#define GAME_HPP
#include "Enemy.hpp"
#include "Hero.hpp"
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>

/// @brief The central point of all game.
class Game
{
protected:
    /// @brief The screen's center on x-axis
    float centerX;
    /// @brief The screen's center on y-axis
    float centerY;
    /// @brief A unique pointer to a list of enemies pointers
    std::unique_ptr<std::list<std::shared_ptr<Enemy>>> enemies;
    /// @brief A pointer to the hero
    std::shared_ptr<Hero> hero;
    /// @brief A unique pointer to game's window
    std::unique_ptr<sf::RenderWindow> gameWindow;
    float spawnInterval = 2;
    float spawnTimer = 0;

public:
    Game(float x, float y) : centerX(x), centerY(y),
                             enemies(std::make_unique<std::list<std::shared_ptr<Enemy>>>()),
                             gameWindow(std::make_unique<sf::RenderWindow>(sf::VideoMode(1200, 800), "Game Window"))
    {
        srand(static_cast<unsigned>(time(0))); // Seeds a random number to game
    };

    static const int windowWidth;
    static const int windowHeight;

    void setHero(std::shared_ptr<Hero> hero) { this->hero = hero; }

    /// @brief Start point to run game.
    void run();

    /// @brief
    /// @return
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