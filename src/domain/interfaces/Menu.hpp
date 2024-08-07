#pragma once // instead of ifndef and define
#include <memory>
#include <SFML/Graphics.hpp>
#include <iostream>

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
    std::unique_ptr<sf::RectangleShape> windowClose;
    std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Texture> image;
    std::unique_ptr<sf::Sprite> bg;
    sf::Vector2f mouseCords;
    sf::Vector2f mousePosition;

    std::unique_ptr<std::vector<const char *>> options;
    std::unique_ptr<std::vector<sf::Vector2f>> optionsCoords;
    std::unique_ptr<std::vector<sf::Text>> optionsTexts;
    std::unique_ptr<std::vector<std::size_t>> optionsSize;

protected:
    void handleEvents();
    void init();
    void drawAll();

public:
    Menu(std::shared_ptr<sf::RenderWindow> gameWindow)
    {
        window = gameWindow;
        windowClose = std::make_unique<sf::RectangleShape>();
        font = std::make_unique<sf::Font>();
        image = std::make_unique<sf::Texture>();
        bg = std::make_unique<sf::Sprite>();
        init();
    }
    ~Menu() = default;
    void run();
};
