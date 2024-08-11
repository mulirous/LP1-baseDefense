#include "interfaces/Menu.hpp"
#include "common.h"
#include <iostream>

void Menu::init()
{
    this->current = 0;
    this->pressed = this->selected = false;

    if (!(this->font->loadFromFile(GAME_FONT)))
    {
        std::cout << "Can't load font :(\n";
    }
    else
    {
        std::cout << "Font loaded successfully.\n";
    }

    if (!(this->image->loadFromFile(MENU_IMAGE)))
    {
        std::cout << "Can't load menu image :(\n";
    }
    else
    {
        std::cout << "Menu image loaded successfully.\n";
    }

    if (!this->bg)
    {
        std::cout << "Background sprite is null.\n";
    }
    else
    {
        this->bg->setTexture(*image);
        std::cout << "Background texture set successfully.\n";
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
        std::cout << "Window pointer valid, scale and position set.\n";
    }
    else
    {
        std::cout << "Window pointer is null.\n";
    }

    this->mousePosition = sf::Vector2f(0, 0);
    this->mouseCords = sf::Vector2f(0, 0);

    this->menuOptions = {
        {"START", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 191)},
        {"ABOUT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 282)},
        {"EXIT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 370)}
    };

    if (this->menuOptions.size() == 0)
    {
        std::cout << "Menu options is empty.\n";
    }
    else
    {
        std::cout << "Menu options initialized successfully.\n";
    }

    // RAPAZEADA, O ERRO TÁ NESSE TRECHO AQUI

    this->options.clear();
    this->options.resize(this->menuOptions.size());

    for (int i = 0; i < this->menuOptions.size(); i++) {
        if (this->font)
        {
            this->options[i].setFont(*font);
        }
        this->options[i].setString(this->menuOptions[i].text);
        this->options[i].setCharacterSize(this->menuOptions[i].size);
        this->options[i].setFillColor(sf::Color::White);
        this->options[i].setPosition(this->menuOptions[i].position);
    }

    if (options.empty()) {
        std::cout << "Options vector is empty after initialization.\n";
    } else {
        std::cout << "Options vector initialized with " << options.size() << " elements.\n";
    }

    options[0].setOutlineThickness(2);

    // FIM DO TRECHO DO ERRO
}

bool Menu::run()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "Window pointer is null in run.\n";
        return true;
    }

    std::cout << "Entering main loop.\n";

    while (windowPtr->isOpen())
    {
        std::cout << "Calling drawAll.\n";
        drawAll();
        std::cout << "Calling handleActions.\n";
        MenuActions action = handleActions();

        switch (action)
        {
        case MenuActions::START:
            std::cout << "Starting the game.\n";
            return false;
        case MenuActions::ABOUT:
            std::cout << "Showing about screen.\n";
            showAbout();
            break;
        case MenuActions::EXIT:
            std::cout << "Exiting the game.\n";
            windowPtr->close();
            break;
        default:
            std::cout << "No action taken.\n";
            break;
        }
    }

    std::cout << "Exiting main loop.\n";
    return true;
}

void Menu::drawAll()
{
    std::cout << "drawAll: Acquired window lock.\n";
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "Window pointer is null in drawAll.\n";
        return;
    }

    std::cout << "drawAll: Clearing window.\n";
    windowPtr->clear();

    std::cout << "drawAll: Drawing background.\n";
    windowPtr->draw(*bg);

    std::cout << "drawAll: Drawing menu options.\n";
    for (const auto &t : this->options)
    {
        windowPtr->draw(t);
    }

    std::cout << "drawAll: Displaying window.\n";
    windowPtr->display();
}

MenuActions Menu::handleActions()
{
    std::cout << "handleActions: Acquired window lock.\n";
    sf::Event menuEvent;
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "Window pointer is null in handleActions.\n";
        return MenuActions::EXIT;
    }

    while (windowPtr->pollEvent(menuEvent))
    {
        std::cout << "handleActions: Polling event.\n";
        switch (menuEvent.type)
        {
        case sf::Event::Closed:
            std::cout << "handleActions: Window closed event.\n";
            windowPtr->close();
            break;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !pressed)
        {
            std::cout << "handleActions: Down key pressed.\n";
            if (this->current < this->options.size() - 1)
            {
                ++this->current;
                this->pressed = true;
                this->options[this->current].setOutlineThickness(2);
                if (this->current > 0)
                {
                    this->options[this->current - 1].setOutlineThickness(0);
                }
                this->pressed = false;
                this->selected = false;
                std::cout << "handleActions: Current option set to " << this->current << ".\n";
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !pressed)
        {
            std::cout << "handleActions: Up key pressed.\n";
            if (this->current > 0)
            {
                --this->current;
                this->pressed = true;
                this->options[this->current].setOutlineThickness(2);
                if (this->current < this->options.size() - 1)
                {
                    this->options[this->current + 1].setOutlineThickness(0);
                }
                this->pressed = false;
                this->selected = false;
                std::cout << "handleActions: Current option set to " << this->current << ".\n";
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
        {
            std::cout << "handleActions: Enter key pressed.\n";
            this->selected = true;
            if (this->current == 0)
                return MenuActions::START;
            if (this->current == 1)
                return MenuActions::ABOUT;
            if (this->current == 2)
                return MenuActions::EXIT;
        }
    }

    std::cout << "handleActions: Exiting.\n";
    return MenuActions::NONE;
}

void Menu::showAbout()
{
    auto windowPtr = window.lock();
    if (!windowPtr)
    {
        std::cout << "Window pointer is null in showAbout.\n";
        return;
    }

    sf::Text infoText;
    if (!font)
    {
        std::cout << "Font is null in showAbout.\n";
    }
    else
    {
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
                    std::cout << "Exiting about screen.\n";
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
}
