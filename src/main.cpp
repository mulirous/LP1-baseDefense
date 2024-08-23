#include "../interfaces/Game.hpp"
#include "../interfaces/Menu.hpp"
#include "../interfaces/Base.hpp"
#include "../enums/GameState.h"
#include "../common.h"
#include "modules/texture_manager/src/ResourceManager.hpp"

const float CENTER_X = GAME_WINDOW_WIDTH / 2;
const float CENTER_Y = GAME_WINDOW_HEIGHT / 2;

void loadResources();

int main()
{
    loadResources();
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1200, 800), "Game Window");

    Menu mainMenu(window);

    // First state is always menu
    GameState currentState = GameState::MENU;

    while (window->isOpen())
    {
        switch (currentState)
        {
        case GameState::MENU:
            if (!mainMenu.run()) // Only change state if run returns false
                currentState = GameState::PLAY;
            break;

        case GameState::PLAY:
        {
            GameDifficulty difficulty = mainMenu.getSelectedDifficulty();
            Game game(CENTER_X, CENTER_Y, window, difficulty);
            game.run();
            currentState = GameState::EXIT; // After game was finished, set state as exit
            break;
        }

        case GameState::EXIT:
            window->close();
            break;
        }
    }

    return EXIT_SUCCESS;
}

void loadResources()
{
    ResourceManager::preLoadTextures(ASSETS_FOLDER, true);
    if (ResourceManager::getNumberOfTextures() <= 0)
    {
        throw std::runtime_error("Assets aren't ready.. \n");
    }
    ResourceManager::preLoadSoundBuffers(AUDIO_FOLDER, true);
    if (ResourceManager::getNumberOfSoundBuffers() <= 0)
    {
        throw std::runtime_error("Audio isn't ready.. \n");
    }
};