#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include <iostream>

using namespace sf;
using namespace std;

const int Game::windowHeight = 800;
const int Game::windowWidth = 1200;

void Game::run()
{
    Clock clock;
    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::render()
{
    gameWindow->clear(Color::White);
    gameWindow->draw(hero->getShape());

    for (const auto &enemy : *enemies)
        gameWindow->draw(enemy->getShape());

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
    }
}

void Game::update(float deltaTime)
{
    hero->move();

    for (const auto &enemy : *enemies)
    {
        enemy->update(deltaTime);
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

    for (auto it = enemies->begin(); it != enemies->end();)
    {
        auto enemy = *it;
        enemy->update(deltaTime);
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
        spawnX = rand() % Game::windowWidth;
        spawnY = -20.f;
        break;
    case 1: // Right
        spawnX = Game::windowWidth + 20.f;
        spawnY = rand() % Game::windowHeight;
        break;
    case 2: // Bottom
        spawnX = rand() % Game::windowWidth;
        spawnY = Game::windowHeight + 20.f;
        break;
    case 3: // Left
        spawnX = -20.f;
        spawnY = rand() % Game::windowHeight;
        break;
    }

    return std::make_shared<Enemy>(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
}

void Game::close()
{
    gameWindow->close();
}