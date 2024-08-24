#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "../enums/MenuActions.h"
#include <SFML/Audio.hpp>
#include "../enums/GameDifficulty.h"
#include "../enums/MenuState.h"
#include "../enums/GameState.h"
#include "MenuOptions.hpp"

/// @file Menu.hpp
/// @class Menu
/// @brief Represents the game's menu system. Includes functionalities such as main menu, difficulty selection and credits section.
class Menu
{
private:
    /// @brief Index of the currently selected menu option.
    int current;

    /// @brief True if the menu has an element currently pressed.
    bool pressed;

    /// @brief True if one or more menu elements are selected.
    bool selected;

    /// @brief Weak pointer to the game window.
    std::weak_ptr<sf::RenderWindow> window;

    /// @brief Unique pointer to the font used for rendering text.
    std::unique_ptr<sf::Font> font;

    /// @brief Unique pointer to the texture used as the menu background.
    std::unique_ptr<sf::Texture> image;

    /// @brief Unique pointer to the sprite that displays the background image.
    std::unique_ptr<sf::Sprite> bg;

    /// @brief Unique pointer to the music played during the game over screen.
    std::unique_ptr<sf::Music> gameOverMusic;

    /// @brief Unique pointer to the music played during the menu screen.
    std::unique_ptr<sf::Music> menuMusic;

    /// @brief Unique pointer to a vector of `MenuOptions` representing main menu options.
    std::unique_ptr<std::vector<MenuOptions>> mainMenuOptions;

    /// @brief Unique pointer to a vector of `MenuOptions` representing difficulty menu options.
    std::unique_ptr<std::vector<MenuOptions>> difficultyMenuOptions;

    /// @brief Unique pointer to a vector of text objects for main menu options.
    std::unique_ptr<std::vector<sf::Text>> mainOptions;

    /// @brief Unique pointer to a vector of text objects for difficulty options.
    std::unique_ptr<std::vector<sf::Text>> difficultyOptions;

    /// @brief The currently selected difficulty level. Default is `MEDIUM`
    GameDifficulty selectedDifficulty = GameDifficulty::MEDIUM;

    /// @brief The current state of the menu. Default is `MAIN`
    MenuState currentState = MenuState::MAIN;

    /// @brief Displays the credits screen with the team informations.
    void displayCredits();

protected:
    /// @brief Handles user actions and interactions with the menu.
    /// @return The action selected by the user.
    /// @note This function also handles navigation through menu options but does not return a `MenuAction` object; it modifies the menu state.
    MenuActions handleActions();

    /// @brief Initializes various objects and sets up the menu.
    void initializeObjects();

    /// @brief Draws the menu options and background on the screen.
    void drawAll();

public:
    /// @brief Constructs the Menu with a reference to the game window.
    /// @param gameWindow A shared pointer to the game window.
    /// @details It loads the musics, font, images and initialize the lists.
    Menu(std::shared_ptr<sf::RenderWindow> gameWindow);

    /// @brief Default Menu destructor.
    virtual ~Menu() = default;

    /// @brief Runs the menu system, handling user interactions and state transitions.
    /// @param state A reference to the current game state.
    /// @param difficulty A reference to the selected game difficulty.
    void run(GameState &state, GameDifficulty &difficulty);

    /// @brief Displays the bad ending screen.
    /// @param state A reference to the current game state.
    void showBadEnding(GameState &state);

    /// @brief Displays the good ending screen.
    /// @param state A reference to the current game state.
    /// @param totalKills The total number of kills in the game.
    void showGoodEnding(GameState &state, int totalKills);
};