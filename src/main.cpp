#include "domain/interfaces/Game.hpp"
#include "domain/interfaces/Menu.hpp"
#include "enums/GameState.h"

const float CENTER_X = 1050.f;
const float CENTER_Y = 700.f;

int main()
{
    auto newHero = std::make_shared<Hero>(50, 50, 5, 100, 600, 400);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1200, 800), "Game Window");
    Menu mainMenu(window);
    Game game(CENTER_X, CENTER_Y, window);
    game.setHero(newHero);

    // First state is always menu
    auto currentState = GameState::MENU;

    while (window->isOpen())
    {
        switch (currentState)
        {
        case GameState::MENU:
            if (!mainMenu.run()) // Only change state if run returns false
                currentState = GameState::PLAY;
            break;

        case GameState::PLAY:
            game.run();
            currentState = GameState::EXIT; // After game was finished, set state as exit
            break;

        case GameState::EXIT:
            window->close();
            break;
        }
    }

    return EXIT_SUCCESS;
}