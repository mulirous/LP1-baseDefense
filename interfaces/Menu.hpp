#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../enums/MenuActions.h"
#include <SFML/Audio.hpp>
#include "../enums/GameDifficulty.h"
#include "../enums/MenuState.h"
#include <iostream>
#include "structs.hpp"

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

    std::shared_ptr<std::vector<sf::Text>> options;
    std::shared_ptr<std::vector<MenuOptions>> mainMenuOptions;
    std::shared_ptr<std::vector<MenuOptions>> difficultyMenuOptions;
    std::shared_ptr<std::vector<sf::Text>> mainOptions;
    std::shared_ptr<std::vector<sf::Text>> difficultyOptions;

    GameDifficulty selectedDifficulty = GameDifficulty::MEDIUM;
    MenuState currentState = MenuState::MAIN;

    /// @brief Show the About Screen with the infos about Us, the developers
    void showAbout();

protected:
    /// @brief Catch user actions with menu
    /// @return The action user selected
    /// @note This also handle interactions such as navigating through options (although this doesn't return any MenuAction object, just modifies the menu state)
    MenuActions handleActions();

    GameDifficulty handleDifficult();

    /// @brief Initialize many objects. Required on constructor
    void init();
    /// @brief Draws options and images on screen
    void drawAll();

public:
    Menu(std::shared_ptr<sf::RenderWindow> gameWindow);
    virtual ~Menu() = default;
    GameDifficulty getSelectedDifficulty() const;
    bool run();
};