#ifndef GAME_HPP
#define GAME_HPP
#include "Enemy.hpp"
#include "Hero.hpp"
#include <list>
#include <SFML/Graphics.hpp>

/// @brief The central point of all game.
class Game
{
protected:
    float centerX;
    float centerY;
    std::list<Enemy> enemies;
    Hero *character;
    sf::RenderWindow gameWindow;
    float spawnInterval = 2;
    float spawnTimer = 0;

public:
    Game(float x, float y) : centerX(x), centerY(y), gameWindow(sf::VideoMode(1200, 800), "Game Window")
    {
        srand(static_cast<unsigned>(time(0))); // Seeds a random number to game
    };

    static const int windowWidth;
    static const int windowHeight;

    void setHero(Hero *hero) { this->character = hero; }

    /// @brief Start point to run game.
    void run();

    /// @brief
    /// @return
    Enemy spawnEnemy();

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