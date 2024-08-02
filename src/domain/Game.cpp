#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>

using namespace sf;
using namespace std;

void Game::run()
{
    Clock clock;
    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
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
            hero->doAttack();
        }
    }
}

void Game::update(float deltaTime)
{
    // Updates everything related to hero
    hero->move();
    auto heroProjectiles = hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles->empty())
    {
        for (auto it = heroProjectiles->begin(); it != heroProjectiles->end(); it++)
        {
            auto projectile = *it;
            if (projectile->isOffScreen())
            {
                it = heroProjectiles->erase(it);
            }
            projectile->update(deltaTime);
        }
    }

    // Updates everything related to enemies
    for (const auto &enemy : *enemies)
    {
        enemy->move(deltaTime);
        // for (const auto &projectile : *enemy->getLaunchedProjectiles())
        // {
        //     if (projectile.expired())
        //         return;
        //     projectile.lock()->update(deltaTime);
        // }
    }

    // Check collisions between hero and enemies
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

    // TODO: refactor this because enemy shouldnt be erased for get at the center of screen
    for (auto it = enemies->begin(); it != enemies->end();)
    {
        auto enemy = *it;
        enemy->move(deltaTime);
        if (abs(enemy->getPosX() - centerX) < 5.f && abs(enemy->getPosY() - centerY) < 5.f)
        {
            it = enemies->erase(it);
        }
        else
        {
            ++it;
        }
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