#include "../interfaces/Game.hpp"
#include "../interfaces/Enemy.hpp"
#include "../common.h"
#include <iostream>
#include <math.h>
#include "../interfaces/Potion.hpp"
#include "../interfaces/Quiver.hpp"
#include "modules/animation/src/Animation.hpp"

Game::Game(float x, float y, std::shared_ptr<sf::RenderWindow> window, GameDifficulty difficulty) :
    centerX(x),
    centerY(y),
    gameWindow(window),
    difficulty(difficulty),
    enemies(std::make_shared<std::list<std::shared_ptr<Enemy>>>()),
    drops(std::make_unique<std::list<std::shared_ptr<Drop>>>()),
    animationManager(std::make_shared<AnimationManager>()),
    background(std::make_unique<sf::Sprite>()),
    menu(std::make_unique<Menu>(window))
{
    srand(static_cast<unsigned>(time(0)));

    int heroHealth;
    float baseDefense;
    float spawnInterval;
    float gameTime;

    switch (difficulty) {
        case GameDifficulty::EASY:
            heroHealth = 125;
            baseDefense = 1000.0f;
            spawnInterval = 5.0f;
            gameTime = 91.0f;
            enemySpd = 30.0f;
            enemyLife = 50;
            break;

        case GameDifficulty::MEDIUM:
            heroHealth = 100;
            baseDefense = 800.0f;
            spawnInterval = 3.0f;
            gameTime = 121.0f;
            enemySpd = 50.0f;
            enemyLife = 80;
            break;

        case GameDifficulty::HARD:
            heroHealth = 75;
            baseDefense = 600.0f;
            spawnInterval = 1.5f;
            gameTime = 181.0f;
            enemySpd = 60.0f;
            enemyLife = 100;
            break;
    }

    hero = std::make_shared<Hero>(heroHealth, 50, 90, 80.0f, 600, 400);
    base = std::make_shared<Base>(baseDefense, x, y);

    hero->initAnimations();

    auto bgTexture = ResourceManager::getTexture(BACKGROUND_GAME);
    sf::Vector2u textureSize = bgTexture->getSize();
    sf::Vector2u windowSize = gameWindow->getSize();
    auto scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    auto scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    background->setTexture(*bgTexture);
    background->setScale(scaleX, scaleY);
    
    this->spawnInterval = spawnInterval;
    this->gameTime = gameTime;
};

void Game::run()
{
    Clock clock;

    while (gameWindow->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        handleEvents();

        if (!isPaused)
        {
            this->gameTime -= deltaTime;

            this->base->heal(this->gameTime);

            setDeltaTime(deltaTime);

            if (base->getLife() <= 0 || hero->getLife() <= 0)
            {
                showGameOver();
                menu->run()
            }
            else if (this->gameTime <= 0)
            {
                showGameWin();
                menu->run()
            }
            else
            {
                render();
                update(deltaTime);
            }
        }
        else
        {
            renderPauseScreen(gameWindow.get());
        }
    }
}

void Game::renderStatus()
{
    auto font = *ResourceManager::getFont(GAME_FONT);

    sf::Text heroLifeText, ammoText, baseLifeText, killCounterText, timeText;
    heroLifeText.setFont(font);
    heroLifeText.setString("LIFE: " + to_string(hero->getLife()));
    heroLifeText.setCharacterSize(16);
    heroLifeText.setFillColor(sf::Color::White);
    heroLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 25);

    ammoText.setFont(font);
    ammoText.setString("MANA: " + to_string(hero->getRangedWeapon()->getAmmo()));
    ammoText.setCharacterSize(16);
    ammoText.setFillColor(sf::Color::White);
    ammoText.setPosition(GAME_WINDOW_WIDTH - 200, 50);

    baseLifeText.setFont(font);
    baseLifeText.setString("BASE: " + to_string(base->getLife()));
    baseLifeText.setCharacterSize(16);
    baseLifeText.setFillColor(sf::Color::White);
    baseLifeText.setPosition(GAME_WINDOW_WIDTH - 200, 75);
    
    killCounterText.setFont(font);
    killCounterText.setString("KILLS: " + to_string(this->killCounter));
    killCounterText.setCharacterSize(16);
    killCounterText.setFillColor(sf::Color::White);
    killCounterText.setPosition(GAME_WINDOW_WIDTH - 200, 100);
    
    int timeToDisplay = static_cast<int>(this->gameTime);

    timeText.setFont(font);
    timeText.setString("TIME: " + to_string(timeToDisplay) + "s");
    timeText.setCharacterSize(16);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(GAME_WINDOW_WIDTH - 200, 125);

    gameWindow->draw(heroLifeText);
    gameWindow->draw(ammoText);
    gameWindow->draw(baseLifeText);
    gameWindow->draw(killCounterText);
    gameWindow->draw(timeText);
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
            isPaused = !isPaused;
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

    auto enemy = std::make_shared<Enemy>(40, 40, enemySpd, enemyLife, spawnX, spawnY, this->centerX, this->centerY);
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
    exitText.setString("Press any key to return");
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
                return;
            }
        }

        gameWindow->clear();
        gameWindow->draw(gameOverText);
        gameWindow->draw(exitText);
        gameWindow->display();
    }
}

void Game::showGameWin()
{
    Font font;
    if (!font.loadFromFile(GAME_FONT))
    {
        std::cout << "Couldn't load font. Exiting.";
        return;
    }

    Text gameWinText, returnText;
    gameWinText.setFont(font);
    gameWinText.setString("You Win!");
    gameWinText.setCharacterSize(48);
    gameWinText.setFillColor(Color::Green);
    gameWinText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 280);

    returnText.setFont(font);
    returnText.setString("Press any key to return");
    returnText.setCharacterSize(24);
    returnText.setFillColor(Color::Black);
    returnText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 350);

    while (gameWindow->isOpen())
    {
        Event event;
        while (gameWindow->pollEvent(event))
        {
            if (event.type == Event::Closed || event.type == Event::KeyPressed)
            {
                return;
            }
        }

        gameWindow->clear(sf::Color::White);
        gameWindow->draw(gameWinText);
        gameWindow->draw(returnText);
        gameWindow->display();
    }
}

void Game::renderPauseScreen(sf::RenderWindow* gameWindow) {
    render();

    sf::Text pauseText;
    pauseText.setFont(*ResourceManager::getFont(GAME_FONT));
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(100, 80);
    gameWindow->draw(pauseText);

    sf::Text escText;
    escText.setFont(*ResourceManager::getFont(GAME_FONT));
    escText.setString("Press ESC to continue");
    escText.setCharacterSize(20);
    escText.setFillColor(sf::Color::White);
    escText.setPosition(100, 280);
    gameWindow->draw(escText);

    gameWindow->display();
}