#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../enums/MenuActions.h"
#include <iostream>
#include <SFML/Audio.hpp>

class Menu
{
private:
    /// @brief Index of current option
    int current;
    /// @brief True if menu has element pressed.
    bool pressed;
    /// @brief True if menu has 1 or more element selected
    bool selected;
    std::weak_ptr<sf::RenderWindow> window;
    std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Texture> image;
    std::unique_ptr<sf::Sprite> bg;
    sf::Vector2f mouseCords;
    sf::Vector2f mousePosition;

    std::unique_ptr<sf::Music> menumusic;

    // TODO: improve this using structs or something
    std::unique_ptr<std::vector<const char *>> options;
    std::unique_ptr<std::vector<sf::Vector2f>> optionsCoords;
    std::unique_ptr<std::vector<sf::Text>> optionsTexts;
    std::unique_ptr<std::vector<std::size_t>> optionsSize;
    /// @brief Show the About Screen with the infos about Us, the developers
    void showAbout();

protected:
    /// @brief Catch user actions with menu
    /// @return The action user selected
    /// @note This also handle interactions such as navigating through options (although this doesn't return any MenuAction object, just modifies the menu state)
    MenuActions handleActions();
    /// @brief Initialize many objects. Required on constructor
    void init();
    /// @brief Draws options and images on screen
    void drawAll();

public:
    Menu(std::shared_ptr<sf::RenderWindow> gameWindow)
    {
        window = gameWindow;
        font = std::make_unique<sf::Font>();
        image = std::make_unique<sf::Texture>();
        bg = std::make_unique<sf::Sprite>();
        menumusic = std::make_unique<sf::Music>();
        init();
    }

    ~Menu() = default;
    bool run();
};
