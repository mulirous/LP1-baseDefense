#include "../interfaces/Menu.hpp"
#include "../common.h"

Menu::Menu(std::shared_ptr<sf::RenderWindow> gameWindow)
{
    window = gameWindow;
    font = std::make_unique<sf::Font>();
    image = std::make_unique<sf::Texture>();
    bg = std::make_unique<sf::Sprite>();
    menumusic = std::make_unique<sf::Music>();
    init();
}

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
        {"EXIT", 24, sf::Vector2f((GAME_WINDOW_WIDTH - 100) / 2, 291)}});

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

    this->menumusic = std::make_unique<sf::Music>();
    if (!this->menumusic->openFromFile(MENU_MUSIC))
    {
        std::cout << "Unable to load the menu music. \n";
    }
    else
    {
        this->menumusic->setLoop(true); // Loop a música
        this->menumusic->play();
    }
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
            this->menumusic->stop();
            return false;
        case MenuActions::ABOUT:
            showAbout();
            break;
        case MenuActions::EXIT:
            this->menumusic->stop();
            windowPtr->close();
            break;
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
    for (const auto &opt : *this->options)
    {
        windowPtr->draw(opt);
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
        switch (menuEvent.type)
        {
        case sf::Event::Closed:
            windowPtr->close();
            break;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !pressed)
        {
            if (this->current < this->options->size() - 1)
            {
                ++this->current;
                this->pressed = true;
                (*this->options)[this->current].setOutlineThickness(2);
                if (this->current > 0)
                {
                    (*this->options)[this->current - 1].setOutlineThickness(0);
                }
                this->pressed = false;
                this->selected = false;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !pressed)
        {
            if (this->current > 0)
            {
                --this->current;
                this->pressed = true;
                (*this->options)[this->current].setOutlineThickness(2);
                if (this->current < this->options->size() - 1)
                {
                    (*this->options)[this->current + 1].setOutlineThickness(0);
                }
                this->pressed = false;
                this->selected = false;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !selected)
        {
            this->selected = true;
            if (this->current == 0)
                return MenuActions::START;
            if (this->current == 1)
                return MenuActions::ABOUT;
            if (this->current == 2)
                return MenuActions::EXIT;
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