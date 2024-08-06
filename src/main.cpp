#include "domain/interfaces/Game.hpp"

const float CENTER_X = 1050.f;
const float CENTER_Y = 700.f;

int main()
{
    auto newHero = std::make_shared<Hero>(50, 50, 15, 100, 600, 400);
    auto newBase = std::make_shared<Base>(50, 500, 500, 550.f, 375.f);
    Game game(CENTER_X, CENTER_Y);
    game.setHero(newHero);
    game.setBase(newBase);
    game.run();

    return EXIT_SUCCESS;
}
