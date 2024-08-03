#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>

using namespace sf;
using namespace std;

Vector2f Game::setMousePosition()
{
    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->gameWindow));
    return mousePosition;
}

void Game::run()
{
    Clock clock;
    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        setDeltaTime(deltaTime);
        render();
        handleEvents();
        update(deltaTime);
    }
}

void Game::render()
{
    gameWindow->clear(Color::White);
    gameWindow->draw(hero->getShape());

    for (const auto &enemy : *enemies)
    {
        gameWindow->draw(enemy->getShape());
        // for (const auto &projectile : *enemy->getLaunchedProjectiles())
        // {
        //     if (projectile.expired())
        //         return;
        //     gameWindow->draw(projectile.lock()->getShape());
        // }
    }

    auto heroProjectiles = *hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles.empty())
    {
        for (const auto &projectile : heroProjectiles)
        {
            gameWindow->draw(projectile->getShape());
        }
    }

    gameWindow->display();
}

void Game::handleEvents()
{
    Event event;
    while (gameWindow->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            gameWindow->close();
        }
        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            gameWindow->close();
        }
        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q)
        {
            auto mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->gameWindow));
            hero->doAttack(mousePosition);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        hero->setTargetPosition(setMousePosition());
    }
}

void Game::update(float deltaTime)
{
    // Updates everything related to hero
    hero->move();
    auto heroProjectiles = hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles->empty())
        this->calculateCollisionsWithProjectiles(heroProjectiles, enemies);

    // Updates everything related to enemies
    for (const auto &enemy : *enemies)
    {
        enemy->move(deltaTime);
    }

    for (const auto &enemy : *enemies)
    {
        if (hero->isCollidingWith(enemy))
        {
            hero->resolveCollision(enemy);
        }

        for (const auto &otherEnemy : *enemies)
        {
            if (&enemy != &otherEnemy && enemy->isCollidingWith(otherEnemy))
            {
                enemy->resolveCollision(otherEnemy);
            }
        }
    }

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval)
    {
        enemies->push_back(this->spawnEnemy());
        spawnTimer = 0.f;
    }
}

std::shared_ptr<Enemy> Game::spawnEnemy()
{
    float spawnX;
    float spawnY;

    int side = rand() % 4;

    switch (side)
    {
    case 0: // Top
        spawnX = rand() % GameWindowWidth;
        spawnY = -20;
        break;
    case 1: // Right
        spawnX = GameWindowWidth + 20;
        spawnY = rand() % GameWindowHeight;
        break;
    case 2: // Bottom
        spawnX = rand() % GameWindowWidth;
        spawnY = GameWindowHeight + 20.f;
        break;
    case 3: // Left
        spawnX = -20.f;
        spawnY = rand() % GameWindowHeight;
        break;
    }

    return std::make_shared<Enemy>(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
}

void Game::close()
{
    gameWindow->close();
}