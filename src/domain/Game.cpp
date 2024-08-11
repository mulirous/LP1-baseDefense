#include "interfaces/Game.hpp"
#include "interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>
#include <math.h>
#include "interfaces/Potion.hpp"
#include "Quiver.hpp"

using namespace sf;
using namespace std;

Game::Game(float x, float y, std::shared_ptr<sf::RenderWindow> window)
{
    srand(static_cast<unsigned>(time(0)));
    centerX = x;
    centerY = y;
    enemies = std::make_shared<std::list<std::shared_ptr<Enemy>>>();
    drops = std::make_unique<std::list<std::shared_ptr<Drop>>>();
    gameWindow = window;
    float windowCenterX = gameWindow->getSize().x / 2.0f;
    float windowCenterY = gameWindow->getSize().y / 2.0f;
    base = std::make_shared<Base>(/*radius=*/50.f, /*maxLife=*/400, /*currentLife=*/400, windowCenterX, windowCenterY);
    animationManager = std::make_shared<AnimationManager>();
};

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
        if (!enemy->isDead())
            gameWindow->draw(enemy->getShape());

        auto enemyProjectiles = *enemy->getRangedWeapon()->getLaunchedProjectiles();
        if (!enemyProjectiles.empty())
        {
            for (const auto &projectile : enemyProjectiles)
            {
                gameWindow->draw(projectile->getShape());
            }
        }
    }

    for (const auto &drop : *drops)
    {
        gameWindow->draw(*drop->getItemSprite());
    }

    auto heroProjectiles = *hero->getRangedWeapon()->getLaunchedProjectiles();
    if (!heroProjectiles.empty())
    {
        for (const auto &projectile : heroProjectiles)
        {
            gameWindow->draw(projectile->getShape());
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
        // Enemy is only erased if enough time has passed; meanwhile, it continues
        if (enemy->isDead())
        {
            if (enemy->getTimeSinceDeath() >= 5)
            {
                enemies->erase(remove(enemies->begin(), enemies->end(), enemy), enemies->end());
                continue;
            }
        }
        else
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
        }

        // Handle enemy's projectiles that are on screen
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

    // Resolve collisions
    for (const auto &enemy : *enemies)
    {
        if (enemy->isDead())
            continue;

        if (hero->isCollidingWith(enemy))
        {
            hero->resolveCollision(enemy);
        }

        if (base->isCollidingWith(enemy->getShape().getGlobalBounds()))
        {
            base->takeDamage(50);
            enemies->erase(std::remove(enemies->begin(), enemies->end(), enemy), enemies->end());
            continue;
        }

        for (const auto &otherEnemy : *enemies)
        {
            if (&enemy != &otherEnemy && enemy->isCollidingWith(otherEnemy))
            {
                enemy->resolveCollision(otherEnemy);
            }
        }
    }

    for (const auto &drop : *drops)
    {
        if (drop->hasExpired())
        {
            drops->erase(std::remove(drops->begin(), drops->end(), drop), drops->end());
            continue;
        }
        if (hero->isCollidingWith(drop->getBounds()))
        {
            auto item = drop->getItem();
            hero->useItem(item);
            drop->setUsed(true);
            continue;
        }
    }

    this->spawnTimer += deltaTime;
    if (this->spawnTimer >= this->spawnInterval)
    {
        spawnEnemy();
        this->spawnTimer = 0.f;
    }
}

void Game::spawnEnemy()
{
    float spawnX;
    float spawnY;

    int side = getRandomNumber(0, 3) % 4;

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

    auto enemy = std::make_shared<Enemy>(40, 40, 50, 80, spawnX, spawnY, this->centerX, this->centerY);
    enemies->push_back(enemy);
}

void Game::close()
{
    gameWindow->close();
}

void Game::spawnDrop(sf::Vector2f &position)
{
    // Randomically selects item
    int num = getRandomNumber(0, 100);

    std::shared_ptr<Drop> drop;

    if (num % 2)
    {
        auto item = std::make_shared<Potion>();
        drop = std::make_shared<Drop>(item, position, DROP_EXPIRATION_SECONDS);
    }
    else
    {
        int arrows = getRandomNumber(5, 10);
        auto item = std::make_shared<Quiver>(arrows);
        drop = std::make_shared<Drop>(item, position, DROP_EXPIRATION_SECONDS);
    }

    drops->push_back(drop);
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