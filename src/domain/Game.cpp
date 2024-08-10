#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>
#include <math.h>

using namespace sf;
using namespace std;

void Game::run()
{
    Clock clock;

    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        setDeltaTime(deltaTime);

        if (base->getLife() <= 0 || hero->getLife() <= 0)
        {
            showGameOver();
            break;
        }
        else
        {
            render();
            handleEvents();
            update(deltaTime);
        }
    }
}
void Game::renderStatus()
{
    Font font = Font();
    if (!font.loadFromFile(GAME_FONT))
    {
        std::cout << "Couldn't load font. Exiting.";
        return;
    }
    Text heroLifeText, ammoText, baseLifeText;
    heroLifeText.setFont(font);
    heroLifeText.setString("LIFE: " + to_string(hero->getLife()));
    heroLifeText.setCharacterSize(16);
    heroLifeText.setFillColor(sf::Color::White);
    heroLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 25);

    ammoText.setFont(font);
    ammoText.setString("AMMO: " + to_string(hero->getRangedWeapon()->getAmmo()));
    ammoText.setCharacterSize(16);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(GAME_WINDOW_WIDTH - 200, 50);

    baseLifeText.setFont(font);
    baseLifeText.setString("BASE: " + to_string(base->getLife()));
    baseLifeText.setCharacterSize(16);
    baseLifeText.setFillColor(sf::Color::White);
    baseLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 75);

    gameWindow->draw(heroLifeText);
    gameWindow->draw(ammoText);
    gameWindow->draw(baseLifeText);
}

void Game::render()
{
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(BACKGROUND_GAME))
    {
        cout << "Background load failed." << endl;
        return;
    }
    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    Vector2u textureSize = backgroundTexture.getSize();
    Vector2u windowSize = gameWindow->getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    backgroundSprite.setScale(scaleX, scaleY);

    gameWindow->clear(Color::White);
    gameWindow->draw(backgroundSprite);
    gameWindow->draw(base->getSprite());
    gameWindow->draw(hero->getSprite());

    for (const auto &enemy : *enemies)
{
    gameWindow->draw(enemy->getShape());
    auto enemyProjectiles = *enemy->getRangedWeapon()->getLaunchedProjectiles();
    if (!enemyProjectiles.empty())
    {
        for (const auto &projectile : enemyProjectiles)
        {
            gameWindow->draw(projectile->getSprite());
        }
    }
}

    auto heroProjectiles = *hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles.empty())
    {
        for (const auto &projectile : heroProjectiles)
        {
            gameWindow->draw(projectile->getSprite());
        }
    }

    this->renderStatus();
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
            sf::Vector2f mousePosition = this->getMousePosition();
            hero->doAttack(mousePosition);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        hero->setTargetPosition(getMousePosition());
    }
}

void Game::update(float deltaTime)
{
    hero->move(deltaTime);
    auto heroProjectiles = hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles->empty())
    {
        for (auto &projectile : *heroProjectiles)
            projectile->update(deltaTime);

        this->calculateCollisionsWithProjectiles(heroProjectiles, enemies);
    }
    // Updates everything related to enemies
    for (const auto &enemy : *enemies)
    {
        enemy->move(deltaTime);
        int randNum = rand();
        if (randNum % 2 == 0)
        {
            if (randNum % 5 == 0)
            {
                auto basePosition = sf::Vector2f(base->getSprite().getPosition());
                enemy->doAttack(basePosition);
            }
            else
            {
                auto heroPosition = sf::Vector2f(hero->getSprite().getPosition());
                enemy->doAttack(heroPosition);
            }
        }
        auto enemyProjectiles = enemy->getRangedWeapon()->getLaunchedProjectiles();
        for (auto &projectile : *enemyProjectiles)
        {
            projectile->update(deltaTime);
            if (base->isCollidingWith(projectile->getBounds()))
            {
                base->takeDamage(projectile->getDamage());
                enemyProjectiles->erase(std::remove(enemyProjectiles->begin(), enemyProjectiles->end(), projectile), enemyProjectiles->end());
            }
            if (hero->isCollidingWith(projectile->getBounds()))
            {
                hero->takeDamage(projectile->getDamage());
                enemyProjectiles->erase(std::remove(enemyProjectiles->begin(), enemyProjectiles->end(), projectile), enemyProjectiles->end());
            }
        }
    }

    for (const auto &enemy : *enemies)
    {
        if (hero->isCollidingWith(enemy))
        {
            hero->resolveCollision(enemy);
        }

        if (base->isCollidingWith(enemy->getShape().getGlobalBounds()))
        {
            base->takeDamage(50);
            enemies->erase(std::remove(enemies->begin(), enemies->end(), enemy), enemies->end());
        }

        for (const auto &otherEnemy : *enemies)
        {
            if (&enemy != &otherEnemy && enemy->isCollidingWith(otherEnemy))
            {
                enemy->resolveCollision(otherEnemy);
            }
        }
    }
    this->spawnTimer += deltaTime;
    if (this->spawnTimer >= this->spawnInterval)
    {
        this->enemies->push_back(this->spawnEnemy());
        this->spawnTimer = 0.f;
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
        spawnX = rand() % GAME_WINDOW_WIDTH;
        spawnY = -20;
        break;
    case 1: // Right
        spawnX = GAME_WINDOW_WIDTH + 20;
        spawnY = rand() % GAME_WINDOW_HEIGHT;
        break;
    case 2: // Bottom
        spawnX = rand() % GAME_WINDOW_WIDTH;
        spawnY = GAME_WINDOW_HEIGHT + 20.f;
        break;
    case 3: // Left
        spawnX = -20.f;
        spawnY = rand() % GAME_WINDOW_HEIGHT;
        break;
    }

    return std::make_shared<Enemy>(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
}

void Game::close()
{
    gameWindow->close();
}

void Game::showGameOver()
{
    Font font;
    if (!font.loadFromFile(GAME_FONT))
    {
        std::cout << "Couldn't load font. Exiting.";
        return;
    }

    Text gameOverText, exitText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 280);

    exitText.setFont(font);
    exitText.setString("Press any key to exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(Color::White);
    exitText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 350);

    while (gameWindow->isOpen())
    {
        Event event;
        while (gameWindow->pollEvent(event))
        {
            if (event.type == Event::Closed || event.type == Event::KeyPressed)
            {
                gameWindow->close();
            }
        }

        gameWindow->clear();
        gameWindow->draw(gameOverText);
        gameWindow->draw(exitText);
        gameWindow->display();
    }
}