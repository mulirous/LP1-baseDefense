#include "../interfaces/Menu.hpp"
#include "modules/texture_manager/src/ResourceManager.hpp"
#include "../common.h"

Menu::Menu(std::shared_ptr<sf::RenderWindow> gameWindow)
{
    window = gameWindow;
    font = std::make_unique<sf::Font>();
    image = std::make_unique<sf::Texture>(*ResourceManager::getTexture(MENU_IMAGE));
    bg = std::make_unique<sf::Sprite>();

    menumusic = std::make_unique<sf::Music>();
    gameOverMusic = std::make_unique<sf::Music>();

    init();
}

void Menu::init()
{
    current = 0;
    pressed = selected = false;
    mousePosition = sf::Vector2f(0, 0);
    mouseCords = sf::Vector2f(0, 0);

    font = std::make_unique<sf::Font>(*ResourceManager::getFont(GAME_FONT));
    bg->setTexture(*image);

    auto windowPtr = window.lock();
    if (windowPtr)
    {
        sf::Vector2u windowSize = windowPtr->getSize();
        sf::Vector2u imageSize = image->getSize();

        float scaleX = static_cast<float>(windowSize.x) / imageSize.x;
        float scaleY = static_cast<float>(windowSize.y) / imageSize.y;
        bg->setScale(scaleX, scaleY);

        float newHeight = imageSize.y * scaleX;
        float offsetY = (windowSize.y - newHeight) / 2;
        bg->setPosition(0, offsetY);
    }

    // Creates the menu's main section options
    mainMenuOptions = std::make_shared<std::vector<MenuOptions>>(std::initializer_list<MenuOptions>{
        {"START", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 150)},
        {"ABOUT", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 100)},
        {"EXIT", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 50)}});

    mainOptions = std::make_shared<std::vector<sf::Text>>(mainMenuOptions->size());

    for (int i = 0; i < mainMenuOptions->size(); i++)
    {
        if (font)
        {
            (*mainOptions)[i].setFont(*font);
        }
        (*mainOptions)[i].setString(mainMenuOptions->at(i).text);
        (*mainOptions)[i].setCharacterSize(mainMenuOptions->at(i).size);
        (*mainOptions)[i].setFillColor(sf::Color::White);
        (*mainOptions)[i].setPosition(mainMenuOptions->at(i).position);
    }
    (*mainOptions)[0].setOutlineThickness(2);

    if (!menumusic->openFromFile(MENU_MUSIC))
    {
        std::cout << "Unable to load the menu music. \n";
    }
    else
    {
        menumusic->setLoop(true);
        menumusic->play();
    }
    gameOverMusic->openFromFile(GAMEOVER_MUSIC);
    gameOverMusic->setLoop(true);
    gameOverMusic->setVolume(100);

    // Creates menu's choose difficult section
    difficultyMenuOptions = std::make_shared<std::vector<MenuOptions>>(std::initializer_list<MenuOptions>{
        {"EASY", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 150)},
        {"MEDIUM", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 100)},
        {"HARD", 24, sf::Vector2f(50, GAME_WINDOW_HEIGHT - 50)}});

    difficultyOptions = std::make_shared<std::vector<sf::Text>>(difficultyMenuOptions->size());

    for (int i = 0; i < difficultyMenuOptions->size(); i++)
    {
        if (font)
        {
            (*difficultyOptions)[i].setFont(*font);
        }
        (*difficultyOptions)[i].setString(difficultyMenuOptions->at(i).text);
        (*difficultyOptions)[i].setCharacterSize(difficultyMenuOptions->at(i).size);
        (*difficultyOptions)[i].setFillColor(sf::Color::White);
        (*difficultyOptions)[i].setPosition(difficultyMenuOptions->at(i).position);
    }

    (*difficultyOptions)[0].setOutlineThickness(2);

    menumusic->openFromFile(MENU_MUSIC);
    menumusic->setLoop(true);
    menumusic->play();
}

void Menu::run(GameState &state, GameDifficulty &difficulty)
{
    auto windowPtr = window.lock();
    if (!windowPtr)
        state = GameState::PLAY;

    while (state != GameState::PLAY && state != GameState::EXIT)
    {
        drawAll();
        MenuActions action = handleActions();

        switch (action)
        {
        case MenuActions::START: // After start, player has to select difficulty
            (*mainOptions)[current].setOutlineThickness(0);
            current = 0;
            (*difficultyOptions)[current].setOutlineThickness(2);
            currentState = MenuState::DIFFICULTY;
            break;

        case MenuActions::CHOOSE_DIFFICULTY:
            difficulty = selectedDifficulty;
            menumusic->stop();
            state = GameState::PLAY; // After difficulty section, game starts
            break;

        case MenuActions::ABOUT:
            showAbout();
            break;

        case MenuActions::EXIT:
            menumusic->stop();
            state = GameState::EXIT;
            break;

        default:
            break;
        }
    }
}

void Menu::drawAll()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
        return;

    windowPtr->clear();
    windowPtr->draw(*bg);

    if (currentState == MenuState::MAIN)
    {
        for (size_t i = 0; i < mainOptions->size(); i++)
        {
            windowPtr->draw((*mainOptions)[i]);
        }
    }
    else if (currentState == MenuState::DIFFICULTY)
    {
        sf::Text returnText;

        if (font)
        {
            returnText.setFont(*font);
        }

        returnText.setString("Press 'Q' to Return");
        returnText.setCharacterSize(16);
        returnText.setFillColor(sf::Color::White);
        returnText.setPosition((GAME_WINDOW_WIDTH - returnText.getLocalBounds().width) / 2, GAME_WINDOW_HEIGHT - returnText.getLocalBounds().height);


        for (size_t i = 0; i < difficultyOptions->size(); i++)
        {
            windowPtr->draw((*difficultyOptions)[i]);
        }

        windowPtr->draw(returnText);
    }

    windowPtr->display();
}

MenuActions Menu::handleActions()
{
    sf::Event menuEvent;
    auto windowPtr = window.lock();
    if (!windowPtr)
        return MenuActions::EXIT;

    while (windowPtr->pollEvent(menuEvent))
    {
        if (menuEvent.type == sf::Event::Closed)
        {
            windowPtr->close();
        }

        if (!pressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (currentState == MenuState::MAIN && current < mainOptions->size() - 1)
                {
                    (*mainOptions)[current].setOutlineThickness(0);
                    ++current;
                    (*mainOptions)[current].setOutlineThickness(2);
                }
                else if (currentState == MenuState::DIFFICULTY && current < difficultyOptions->size() - 1)
                {
                    (*difficultyOptions)[current].setOutlineThickness(0);
                    ++current;
                    (*difficultyOptions)[current].setOutlineThickness(2);
                }
                pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentState == MenuState::MAIN && current > 0)
                {
                    (*mainOptions)[current].setOutlineThickness(0);
                    --current;
                    (*mainOptions)[current].setOutlineThickness(2);
                }
                else if (currentState == MenuState::DIFFICULTY && current > 0)
                {
                    (*difficultyOptions)[current].setOutlineThickness(0);
                    --current;
                    (*difficultyOptions)[current].setOutlineThickness(2);
                }
                pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
            {
                selected = true;
                pressed = true;

                if (currentState == MenuState::MAIN)
                {
                    if (current == 0)
                        return MenuActions::START;
                    if (current == 1)
                        return MenuActions::ABOUT;
                    if (current == 2)
                        return MenuActions::EXIT;
                }
                else if (currentState == MenuState::DIFFICULTY)
                {
                    if (current == 0)
                        selectedDifficulty = GameDifficulty::EASY;
                    if (current == 1)
                        selectedDifficulty = GameDifficulty::MEDIUM;
                    if (current == 2)
                        selectedDifficulty = GameDifficulty::HARD;

                    return MenuActions::CHOOSE_DIFFICULTY;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && currentState == MenuState::DIFFICULTY)
            {
                currentState = MenuState::MAIN;
                (*difficultyOptions)[current].setOutlineThickness(0);
                current = 0;
                (*mainOptions)[current].setOutlineThickness(2);
            }
        }

        if (menuEvent.type == sf::Event::KeyReleased)
        {
            pressed = false;
            selected = false;
        }
    }

    return MenuActions::NONE;
}

void Menu::showAbout()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
        return;

    sf::Text infoText;
    infoText.setFont(*font);
    infoText.setCharacterSize(15);
    infoText.setFillColor(sf::Color::White);

    std::string aboutText =
        "Game Version 1.0\n"
        "Developed by\n\n\n"
        "Andriel Vinicius\n\n"
        "   Performance Optimizer, Modularizer, Game Designer\n"
        "   & Developer\n"
        "\n\n"
        "Flawbert Lorran\n\n"
        "   Game Designer, Sound Engineer & Developer\n"
        "\n\n"
        "Murilo Costa\n\n"
        "   Performance Optimizer, Modularizer & Developer\n"
        "\n\n"
        "Special thanks to Hadassa Garcia for created the ending background."
        "\n\n";

    infoText.setString(aboutText);
    infoText.setPosition(100.f, 200.f);

    sf::Text exitText;
    exitText.setFont(*font);
    exitText.setCharacterSize(18);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Press `Q` to return");
    exitText.setPosition(420.f, 600.f);

    while (windowPtr->isOpen())
    {
        sf::Event event;
        while (windowPtr->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                return;
            }
        }

        windowPtr->clear();
        windowPtr->draw(*bg);
        windowPtr->draw(infoText);
        windowPtr->draw(exitText);
        windowPtr->display();
    }
}

void Menu::showBadEnding(GameState &state)
{
    auto windowPtr = window.lock();

    if (!windowPtr)
        return;

    gameOverMusic->play();

    auto gameOverImage = ResourceManager::getTexture(BAD_GAMEOVER_IMAGE);
    sf::Sprite gameOverBg;
    if (gameOverImage)
    {
        gameOverBg.setTexture(*gameOverImage);

        sf::Vector2u windowSize = windowPtr->getSize();
        sf::Vector2u imageSize = gameOverImage->getSize();

        float scaleX = static_cast<float>(windowSize.x) / imageSize.x;
        float scaleY = static_cast<float>(windowSize.y) / imageSize.y;
        gameOverBg.setScale(scaleX, scaleY);

        float newHeight = imageSize.y * scaleX;
        float offsetY = (windowSize.y - newHeight) / 2;
        gameOverBg.setPosition(0, offsetY);
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
    exitText.setString("Press any key to return");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition((GAME_WINDOW_WIDTH / 3) - (GAME_WINDOW_WIDTH / 5), 350);

    while (state != GameState::MENU)
    {
        sf::Event event;
        while (windowPtr->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
            {
                gameOverMusic->stop();
                state = GameState::MENU;
                break;
            }
        }

        windowPtr->clear();
        windowPtr->draw(gameOverBg);
        windowPtr->draw(gameOverText);
        windowPtr->draw(exitText);
        windowPtr->display();
    }

    return;
}

void Menu::showGoodEnding(GameState &state, int totalKills)
{
    auto windowPtr = window.lock();

    if (!windowPtr)
        return;

    gameOverMusic->play();

    sf::Font font = *ResourceManager::getFont(GAME_FONT);

    sf::Text gameWinText, killCounterText, returnText;
    gameWinText.setFont(font);
    gameWinText.setString("You Win!");
    gameWinText.setCharacterSize(48);
    gameWinText.setFillColor(sf::Color::Green);
    gameWinText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 280);

    killCounterText.setFont(font);
    killCounterText.setString("Kills: " + std::to_string(totalKills));
    killCounterText.setCharacterSize(24);
    killCounterText.setFillColor(sf::Color::Black);
    killCounterText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 350);

    returnText.setFont(font);
    returnText.setString("Press any key to return");
    returnText.setCharacterSize(24);
    returnText.setFillColor(sf::Color::Black);
    returnText.setPosition((GAME_WINDOW_WIDTH / 2) - (GAME_WINDOW_WIDTH / 5), 420);

    while (state != GameState::MENU)
    {
        sf::Event event;
        while (windowPtr->pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
            {
                gameOverMusic->stop();
                state = GameState::MENU;
                break;
            }
        }

        windowPtr->clear(sf::Color::White);
        windowPtr->draw(gameWinText);
        windowPtr->draw(killCounterText);
        windowPtr->draw(returnText);
        windowPtr->display();
    }
}