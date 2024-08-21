#include "../interfaces/Menu.hpp"
#include "../common.h"
#include <iostream>

void Menu::init()
{
    this->current = 0;
    this->pressed = this->selected = false;

    if (!(this->font->loadFromFile(GAME_FONT)))
    {
        std::cout << "Can't load font :(\n";
    }

    if (!(this->image->loadFromFile(MENU_IMAGE)))
    {
        std::cout << "Can't load menu image :(\n";
    }

    if (this->bg)
    {
        this->bg->setTexture(*image);
    }

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

    this->mousePosition = sf::Vector2f(0, 0);
    this->mouseCords = sf::Vector2f(0, 0);

        this->mainMenuOptions = std::make_shared<std::vector<MenuOptions>>(std::initializer_list<MenuOptions>{
        {"START", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 191)},
        {"ABOUT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 241)},
        {"EXIT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 291)}
    });

    this->mainOptions = std::make_shared<std::vector<sf::Text>>(this->mainMenuOptions->size());

    for (int i = 0; i < this->mainMenuOptions->size(); i++)
    {
        if (this->font)
        {
            (*this->mainOptions)[i].setFont(*font);
        }
        (*this->mainOptions)[i].setString(this->mainMenuOptions->at(i).text);
        (*this->mainOptions)[i].setCharacterSize(this->mainMenuOptions->at(i).size);
        (*this->mainOptions)[i].setFillColor(sf::Color::White);
        (*this->mainOptions)[i].setPosition(this->mainMenuOptions->at(i).position);
    }

    (*mainOptions)[0].setOutlineThickness(2);

    this->difficultyMenuOptions = std::make_shared<std::vector<MenuOptions>>(std::initializer_list<MenuOptions>{
        {"EASY", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 191)},
        {"MEDIUM", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 241)},
        {"HARD", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 291)}
    });

    this->difficultyOptions = std::make_shared<std::vector<sf::Text>>(this->difficultyMenuOptions->size());

    for (int i = 0; i < this->difficultyMenuOptions->size(); i++)
    {
        if (this->font)
        {
            (*this->difficultyOptions)[i].setFont(*font);
        }
        (*this->difficultyOptions)[i].setString(this->difficultyMenuOptions->at(i).text);
        (*this->difficultyOptions)[i].setCharacterSize(this->difficultyMenuOptions->at(i).size);
        (*this->difficultyOptions)[i].setFillColor(sf::Color::White);
        (*this->difficultyOptions)[i].setPosition(this->difficultyMenuOptions->at(i).position);
    }

    (*difficultyOptions)[0].setOutlineThickness(2);
}

bool Menu::run()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
        return true;

    while (windowPtr->isOpen())
    {
        drawAll();
        MenuActions action = handleActions();

        switch (action)
        {
        case MenuActions::START:
            currentState = MenuState::DIFFICULTY;
            (*this->mainOptions)[this->current].setOutlineThickness(0);
            this->current = 0;
            (*this->difficultyOptions)[this->current].setOutlineThickness(2);
            break;
        case MenuActions::ABOUT:
            showAbout();
            break;
        case MenuActions::EXIT:
            windowPtr->close();
            break;
        case MenuActions::CHOOSE_DIFFICULTY:
            return false;
        default:
            break;
        }
    }

    return true;
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
        for (size_t i = 0; i < mainOptions->size(); ++i)
        {
            windowPtr->draw((*this->mainOptions)[i]);
        }
    }
    else if (currentState == MenuState::DIFFICULTY)
    {
        sf::Text enterText, returnText;

        if (font) {
            enterText.setFont(*font);
            returnText.setFont(*font);
        }

        enterText.setString("Press Enter Three Times");
        enterText.setCharacterSize(16);
        enterText.setFillColor(sf::Color::White);
        enterText.setPosition((GAME_WINDOW_WIDTH - enterText.getLocalBounds().width) / 2, 391);

        returnText.setString("Press 'Q' to Return");
        returnText.setCharacterSize(16);
        returnText.setFillColor(sf::Color::White);
        returnText.setPosition((GAME_WINDOW_WIDTH - returnText.getLocalBounds().width) / 2, 491);

        for (size_t i = 0; i < this->difficultyOptions->size(); ++i)
        {
            windowPtr->draw((*this->difficultyOptions)[i]);
        }

        windowPtr->draw(enterText);
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
                if (currentState == MenuState::MAIN && this->current < this->mainOptions->size() - 1)
                {
                    (*this->mainOptions)[this->current].setOutlineThickness(0);
                    ++this->current;
                    (*this->mainOptions)[this->current].setOutlineThickness(2);
                }
                else if (currentState == MenuState::DIFFICULTY && this->current < this->difficultyOptions->size() - 1)
                {
                    (*this->difficultyOptions)[this->current].setOutlineThickness(0);
                    ++this->current;
                    (*this->difficultyOptions)[this->current].setOutlineThickness(2);
                }
                this->pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (currentState == MenuState::MAIN && this->current > 0)
                {
                    (*this->mainOptions)[this->current].setOutlineThickness(0);
                    --this->current;
                    (*this->mainOptions)[this->current].setOutlineThickness(2);
                }
                else if (currentState == MenuState::DIFFICULTY && this->current > 0)
                {
                    (*this->difficultyOptions)[this->current].setOutlineThickness(0);
                    --this->current;
                    (*this->difficultyOptions)[this->current].setOutlineThickness(2);
                }
                this->pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
            {
                this->selected = true;
                this->pressed = true;

                if (currentState == MenuState::MAIN)
                {
                    if (this->current == 0)
                        return MenuActions::START;
                    if (this->current == 1)
                        return MenuActions::ABOUT;
                    if (this->current == 2)
                        return MenuActions::EXIT;
                }
                else if (currentState == MenuState::DIFFICULTY)
                {
                    if (this->current == 0)
                        this->selectedDifficulty = GameDifficulty::EASY;
                    if (this->current == 1)
                        this->selectedDifficulty = GameDifficulty::MEDIUM;
                    if (this->current == 2)
                        this->selectedDifficulty = GameDifficulty::HARD;

                    return MenuActions::CHOOSE_DIFFICULTY;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && currentState == MenuState::DIFFICULTY)
            {
                currentState = MenuState::MAIN;
                (*this->difficultyOptions)[this->current].setOutlineThickness(0);
                this->current = 0;
                (*this->mainOptions)[this->current].setOutlineThickness(2);
            }
        }

        if (menuEvent.type == sf::Event::KeyReleased)
        {
            this->pressed = false;
            this->selected = false;
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
        "   Performance Optimizer, Modularizer, Game Designer, Sound Engineer\n"
        "   & Developer\n"
        "\n\n"
        "Flawbert Lorran\n\n"
        "   Game Designer, Sound Engineer & Developer\n"
        "\n\n"
        "Murilo Costa\n\n"
        "   Performance Optimizer, Modularizer & Developer\n";

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
            // Exiting 'about' screen
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