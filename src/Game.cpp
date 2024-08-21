#include "../interfaces/Game.hpp"
#include "../interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>
#include <math.h>
#include "../interfaces/Potion.hpp"
#include "../interfaces/Quiver.hpp"
#include "modules/animation/src/Animation.hpp"

Game::Game(float x, float y, std::shared_ptr<sf::RenderWindow> window)
{
    srand(static_cast<unsigned>(time(0)));
    centerX = x;
    centerY = y;
    enemies = std::make_shared<std::list<std::shared_ptr<Enemy>>>();
    drops = std::make_unique<std::list<std::shared_ptr<Drop>>>();
    gameWindow = window;
    hero = std::make_shared<Hero>(50, 50, 90, 100, 600, 400);
    hero->initAnimations();
    base = std::make_shared<Base>(500, x, y);
    background = std::make_unique<sf::Sprite>();

    auto bgTexture = ResourceManager::getTexture(BACKGROUND_GAME);
    sf::Vector2u textureSize = bgTexture->getSize();
    sf::Vector2u windowSize = gameWindow->getSize();
    auto scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    auto scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    background->setTexture(*bgTexture);
    background->setScale(scaleX, scaleY);
};

void Game::run()
{
    sf::Clock clock;

    this->battlemusic = std::make_unique<sf::Music>();
    if (!this->battlemusic->openFromFile(BATTLE_MUSIC))
    {
        std::cout << "Unable to load the battle music. \n";
    }
    else
    {
        this->battlemusic->setLoop(true); // Loop a música
        this->battlemusic->play();
    }

    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        setDeltaTime(deltaTime);

        if (base->getLife() <= 0 || hero->getLife() <= 0)
        {
            this->battlemusic->stop();
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
    auto font = *ResourceManager::getFont(GAME_FONT);

    sf::Text heroLifeText, ammoText, baseLifeText;
    heroLifeText.setFont(font);
    heroLifeText.setString("LIFE: " + std::to_string(hero->getLife()));
    heroLifeText.setCharacterSize(16);
    heroLifeText.setFillColor(sf::Color::White);
    heroLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 25);

    ammoText.setFont(font);
    ammoText.setString("MANA: " + std::to_string(hero->getRangedWeapon()->getAmmo()));
    ammoText.setCharacterSize(16);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(GAME_WINDOW_WIDTH - 200, 50);

    baseLifeText.setFont(font);
    baseLifeText.setString("BASE: " + std::to_string(base->getLife()));
    baseLifeText.setCharacterSize(16);
    baseLifeText.setFillColor(sf::Color::White);
    baseLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 75);

    gameWindow->draw(heroLifeText);
    gameWindow->draw(ammoText);
    gameWindow->draw(baseLifeText);
}

void Game::render()
{
    gameWindow->clear(sf::Color::White);
    gameWindow->draw(*background);
    gameWindow->draw(*base->getSprite());
    gameWindow->draw(*hero->getSprite());

    for (const auto &enemy : *enemies)
    {
        if (!enemy->isDead())
            gameWindow->draw(*enemy->getSprite());

        auto enemyProjectiles = *enemy->getRangedWeapon()->getLaunchedProjectiles();
        if (!enemyProjectiles.empty())
        {
            for (const auto &projectile : enemyProjectiles)
            {
                gameWindow->draw(projectile->getSprite());
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
            gameWindow->draw(projectile->getSprite());
        }
    }

    this->renderStatus();
    gameWindow->display();
}

void Game::handleEvents()
{
    sf::Event event;
    while (gameWindow->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            gameWindow->close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            gameWindow->close();
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
        {
            sf::Vector2f mousePosition = this->getMousePosition();
            hero->doAttack(mousePosition, this->deltaTime);
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

    for (const auto &enemy : *enemies)
    {
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
            enemy->updateAnimation("walk", deltaTime); // Atualiza a animação de movimento
            int randNum = rand();
            if (randNum % 2 == 0)
            {
                if (randNum % 5 == 0)
                {
                    auto basePosition = sf::Vector2f(base->getSprite()->getPosition());
                    enemy->doAttack(basePosition);
                }
                else
                {
                    auto heroPosition = sf::Vector2f(hero->getSprite()->getPosition());
                    enemy->doAttack(heroPosition);
                }
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
    // Resolve collisions
    for (const auto &enemy : *enemies)
    {
        if (enemy->isDead())
            continue;

        if (hero->isCollidingWith(enemy))
        {
            hero->resolveCollision(enemy);
        }

        if (base->isCollidingWith(enemy->getSprite()->getGlobalBounds()))
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
        drop->getItem()->animate(deltaTime);
        if (hero->isCollidingWith(drop->getBounds()))
        {
            auto item = drop->getItem();
            hero->useItem(item);
            drop->markAsUsed();
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
        int mana = getRandomNumber(5, 10);
        auto item = std::make_shared<ManaPotion>(mana);
        drop = std::make_shared<Drop>(item, position, DROP_EXPIRATION_SECONDS);
    }

    drops->push_back(drop);
}

void Game::showGameOver()
{
    this->gameovermusic = std::make_unique<sf::Music>();
    if (!this->gameovermusic->openFromFile(GAMEOVER_MUSIC))
    {
        std::cout << "Unable to load the game over music. \n";
    }
    else
    {
        this->gameovermusic->setLoop(true); // Loop a música
        this->gameovermusic->play();
    }

    sf::Font font;
    if (!font.loadFromFile(GAME_FONT))
    {
        std::cout << "Couldn't load font. Exiting.";
        return;
    }

    sf::Text gameOverText, exitText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 280);

    exitText.setFont(font);
    exitText.setString("Press any key to exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 350);

    while (gameWindow->isOpen())
    {
        sf::Event event;
        while (gameWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
            {
                gameWindow->close();
                this->gameovermusic->stop();
            }
        }

        gameWindow->clear();
        gameWindow->draw(gameOverText);
        gameWindow->draw(exitText);
        gameWindow->display();
    }
}