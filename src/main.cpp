#include "domain/interfaces/Game.hpp"

#define CENTER_X = 1050.f;
#define CENTER_Y = 700.f;

int main()
{
    auto character = new Hero(50, 50, 50, 100, 600, 400);
    auto game = new Game(1050, 700);
    game->setHero(character);
    game->run();

    return EXIT_SUCCESS;
}
