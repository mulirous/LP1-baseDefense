#include "interfaces/Menu.hpp"
#include "common.h"
#include <iostream>

void Menu::init()
{
    this->current = 0;
    this->pressed = this->selected = false;
    if (!(this->font->loadFromFile(GAMEFONT)))
    {
        std::cout << "Can't load font :(\n ";
    };
    if (!(this->image->loadFromFile(MENUIMAGE)))
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
    this->optionsCoords = std::make_unique<std::vector<sf::Vector2f>>(std::initializer_list<sf::Vector2f>{{610, 191}, {590, 282}, {600, 370}});

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

void Menu::handleEvents()
{
    sf::Event menuEvent;
    auto windowPtr = window.lock();
    if (!windowPtr)
        return;

    while (windowPtr->pollEvent(menuEvent))
    {
        switch (menuEvent.type)
        {
        case sf::Event::Closed:
            windowPtr->close();
            break;
        }
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
            // If user enters on quit option (the last one)
            if (this->current == (int)this->optionsTexts->size() - 1)
            {
                auto windowPtr = window.lock();
                if (windowPtr)
                    windowPtr->close();
            }
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
    for (auto t : *this->optionsTexts)
    {
        windowPtr->draw(t);
    }
    windowPtr->display();
}

void Menu::run()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "WINDOW CRASH";
        return;
    }

    while (windowPtr->isOpen())
    {
        drawAll();
        handleEvents();
    }
}