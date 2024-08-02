#include "domain/interfaces/Game.hpp"

const float CENTER_X = 1050.f;
const float CENTER_Y = 700.f;

int main()
{
    auto newHero = std::make_shared<Hero>(50, 50, 5, 100, 600, 400);
    Game game(CENTER_X, CENTER_Y);
    game.setHero(newHero);
    game.run();

    return EXIT_SUCCESS;
}
