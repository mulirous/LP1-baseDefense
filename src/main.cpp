#include "../common.h"
#include "../interfaces/Game.hpp"
#include "modules/texture_manager/src/ResourceManager.hpp"

void loadResources();

int main()
{
    loadResources();
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1200, 800), "SpellGuard");

    Game game(window);

    game.start();

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