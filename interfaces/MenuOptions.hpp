#pragma once

#include <SFML/Graphics.hpp>

/// @brief Represents each menu options, encapsulating relevant attributes to display it on screen.
struct MenuOptions
{
    std::string text;
    size_t size;
    sf::Vector2f position;
};
