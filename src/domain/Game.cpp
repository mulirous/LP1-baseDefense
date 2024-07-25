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
    while (gameWindow.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::render()
{
    gameWindow.clear(Color::White);
    gameWindow.draw(character->getShape());

    for (auto &enemy : enemies)
        gameWindow.draw(enemy.getShape());

    gameWindow.display();
}

void Game::handleEvents()
{
    Event event;
    while (gameWindow.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            gameWindow.close();
        }
        else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
        {
            gameWindow.close();
        }
    }
}

void Game::update(float deltaTime)
{
    character->move();

    // Atualizar inimigos
    for (auto &enemy : enemies)
    {
        enemy.update(deltaTime);
    }

    // Verificar colisões entre inimigos e herói
    for (auto &enemy : enemies)
    {
        if (character->isCollidingWith(enemy))
        {
            character->resolveCollision(enemy);
        }

        for (auto &otherEnemy : enemies)
        {
            if (&enemy != &otherEnemy && enemy.isCollidingWith(otherEnemy))
            {
                enemy.resolveCollision(otherEnemy);
            }
        }
    }

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval)
    {
        enemies.push_back(this->spawnEnemy());
        spawnTimer = 0.f;
    }

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        it->update(deltaTime);
        if (abs(it->getPosX() - centerX) < 5.f && abs(it->getPosY() - centerY) < 5.f)
        {
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

Enemy Game::spawnEnemy()
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

    return Enemy(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
}

void Game::close()
{
    gameWindow.close();
}