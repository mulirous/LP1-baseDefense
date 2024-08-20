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

    this->menuOptions = std::make_shared<std::vector<MenuOptions>>(std::initializer_list<MenuOptions>{
        {"START", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 191)},
        {"ABOUT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 241)},
        {"EXIT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 291)},
        {"EASY", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 191)},
        {"MEDIUM", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 241)},
        {"HARD", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 291)}
    });

    this->options = std::make_shared<std::vector<sf::Text>>(this->menuOptions->size());

    for (int i = 0; i < this->menuOptions->size(); i++)
    {
        if (this->font)
        {
            (*this->options)[i].setFont(*font);
        }
        (*this->options)[i].setString(this->menuOptions->at(i).text);
        (*this->options)[i].setCharacterSize(this->menuOptions->at(i).size);
        (*this->options)[i].setFillColor(sf::Color::White);
        (*this->options)[i].setPosition(this->menuOptions->at(i).position);
    }

    (*options)[0].setOutlineThickness(2);
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
            // Reset the current selection to the first difficulty option
            (*this->options)[this->current].setOutlineThickness(0); // Clear previous selection
            this->current = 3; // Set to the first difficulty option
            (*this->options)[this->current].setOutlineThickness(2);
            break;
        case MenuActions::ABOUT:
            showAbout();
            break;
        case MenuActions::EXIT:
            windowPtr->close();
            break;
        case MenuActions::CHOOSE_DIFFICULTY:
            return false;  // Começa o jogo
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
        for (size_t i = 0; i < 3; ++i)
        {
            windowPtr->draw((*this->options)[i]);
        }
    }
    else if (currentState == MenuState::DIFFICULTY)
    {   
        sf::Text enterText, returnText;

        // Certifique-se de que a fonte está carregada e atribuída corretamente
        if (font) {
            enterText.setFont(*font);
            returnText.setFont(*font);
        } else {
            std::cerr << "Fonte não carregada corretamente!" << std::endl;
            return;
        }

        enterText.setString("Press Enter Three Times");
        enterText.setCharacterSize(16);
        enterText.setFillColor(sf::Color::White);
        // Centralizar o texto na tela
        enterText.setPosition((GAME_WINDOW_WIDTH - enterText.getLocalBounds().width) / 2, 391);

        returnText.setString("Press 'Q' to Return");
        returnText.setCharacterSize(16);
        returnText.setFillColor(sf::Color::White);
        // Centralizar o texto na tela
        returnText.setPosition((GAME_WINDOW_WIDTH - returnText.getLocalBounds().width) / 2, 491);

        for (size_t i = 3; i < this->options->size(); ++i)
        {
            windowPtr->draw((*this->options)[i]);
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
                if (this->current < this->options->size() - 1)
                {
                    (*this->options)[this->current].setOutlineThickness(0);
                    ++this->current;
                    (*this->options)[this->current].setOutlineThickness(2);
                }
                this->pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (this->current > 0)
                {
                    (*this->options)[this->current].setOutlineThickness(0);
                    --this->current;
                    (*this->options)[this->current].setOutlineThickness(2);
                }
                this->pressed = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
            {
                this->selected = true;
                this->pressed = true;

                if (currentState == MenuState::MAIN)
                {
                    if (this->current == 0) // START
                        return MenuActions::START;
                    if (this->current == 1) // ABOUT
                        return MenuActions::ABOUT;
                    if (this->current == 2) // EXIT
                        return MenuActions::EXIT;
                }
                else if (currentState == MenuState::DIFFICULTY)
                {   
                    if (this->current == 3)
                        this->selectedDifficulty = GameDifficulty::EASY;
                    if (this->current == 4)
                        this->selectedDifficulty = GameDifficulty::MEDIUM;
                    if (this->current == 5)
                        this->selectedDifficulty = GameDifficulty::HARD;

                    return MenuActions::CHOOSE_DIFFICULTY;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && currentState == MenuState::DIFFICULTY)
            {
                currentState = MenuState::MAIN;
                (*this->options)[this->current].setOutlineThickness(0);
                this->current = 0; // Reset para o primeiro item do menu principal
                (*this->options)[this->current].setOutlineThickness(2);
            }
        }
        
        if (menuEvent.type == sf::Event::KeyReleased)
        {
            this->pressed = false;
            this->selected = false; // Permitir nova seleção após a tecla ser liberada
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