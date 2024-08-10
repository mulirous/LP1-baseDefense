#include "interfaces/Menu.hpp"
#include "common.h"
#include <vector>
#include <iostream>

void Menu::init()
{
    this->current = 0;
    this->pressed = this->selected = false;
    if (!(this->font->loadFromFile(GAME_FONT)))
    {
        std::cout << "Can't load font :(\n ";
    };
    if (!(this->image->loadFromFile(MENU_IMAGE)))
    {
        std::cout << "Can't load menu image :( \n";
    };
    this->bg->setTexture(*image);
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
    };

    this->mousePosition = sf::Vector2f(0, 0);
    this->mouseCords = sf::Vector2f(0, 0);
    this->options = std::make_unique<std::vector<const char *>>(std::initializer_list<const char *>{"START", "ABOUT", "EXIT"});
    this->optionsSize = std::make_unique<std::vector<std::size_t>>(std::initializer_list<std::size_t>{24, 24, 24});
    this->optionsCoords = std::make_unique<std::vector<sf::Vector2f>>(std::initializer_list<sf::Vector2f>{{(GAME_WINDOW_WIDTH - 100) / 2, 191}, {(GAME_WINDOW_WIDTH - 100) / 2, 282}, {(GAME_WINDOW_WIDTH - 100) / 2, 370}});

    this->optionsTexts = std::make_unique<std::vector<sf::Text>>();
    this->optionsTexts->resize(options->size());

    for (std::size_t i = 0; i < optionsTexts->size(); i++)
    {
        (*optionsTexts)[i].setFont(*font);
        (*optionsTexts)[i].setString((*options)[i]);
        (*optionsTexts)[i].setCharacterSize((*optionsSize)[i]);
        (*optionsTexts)[i].setOutlineColor(sf::Color::White);
        (*optionsTexts)[i].setPosition((*optionsCoords)[i]);
    }
    (*optionsTexts)[0].setOutlineThickness(2); // Always start on 'Start'
}

MenuActions Menu::handleActions()
{
    sf::Event menuEvent;
    auto windowPtr = window.lock();
    if (!windowPtr)
        return MenuActions::EXIT; // Error ocurred

    while (windowPtr->pollEvent(menuEvent))
    {
        switch (menuEvent.type)
        {
        case sf::Event::Closed:
            windowPtr->close();
            break;
        }
        // TODO: cleanup this code and use some switchs
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !pressed)
        {
            // If there is more options below, advance once and highlight new option
            if (this->current < (int)this->optionsTexts->size() - 1)
            {
                ++this->current;
                this->pressed = true;
                (*this->optionsTexts)[this->current].setOutlineThickness(2);
                (*this->optionsTexts)[this->current - 1].setOutlineThickness(0);
                this->pressed = false;
                this->selected = false;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !pressed)
        {
            // If there is more options above, advance once and hightlight new one
            if (this->current >= 1)
            {
                --this->current;
                this->pressed = true;
                (*this->optionsTexts)[this->current].setOutlineThickness(2);
                (*this->optionsTexts)[this->current + 1].setOutlineThickness(0);
                this->pressed = false;
                this->selected = false;
            }
        }
        // Allows to enter only once on the option because of the selected field
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
        {
            this->selected = false;
            // If user enters on start option, it should start the game
            if (this->current == 0)
                return MenuActions::START;
            
            // If user enters on start option, it should show the about
            if (this->current == (int)this->optionsTexts->size() - 2)
                return MenuActions::ABOUT;

            // If user enters on quit option (the last one)
            if (this->current == (int)this->optionsTexts->size() - 1)
                return MenuActions::EXIT;
        }
    }
    return MenuActions::NONE; // Don't do anything
}

void Menu::drawAll()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
        return;

    windowPtr->clear();
    windowPtr->draw(*bg);
    for (auto t : *this->optionsTexts)
    {
        windowPtr->draw(t);
    }
    windowPtr->display();
}

bool Menu::run()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "WINDOW CRASH";
        return true;
    }

    while (windowPtr->isOpen())
    {
        drawAll();
        MenuActions action = handleActions();

        // Only breaks up the menu loop if player selected start option
        switch (action)
        {
        case MenuActions::START:
            return false;
        case MenuActions::ABOUT:
            showAbout();
            break;
        case MenuActions::EXIT:
            windowPtr->close();
            break;
        }
    }
    // if nothing happens, stays on menu
    return true;
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
