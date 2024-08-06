#include "domain/interfaces/Game.hpp"

const float WINDOW_WIDTH = 1200.f;
const float WINDOW_HEIGHT = 800.f;
const float CENTER_X = WINDOW_WIDTH / 2;
const float CENTER_Y = WINDOW_HEIGHT / 2;

int main()
{
    auto newHero = std::make_shared<Hero>(50, 50, 15, 100, 600, 400);
    auto newBase = std::make_shared<Base>(50, 500, 500, CENTER_X, CENTER_Y);
    Game game(CENTER_X, CENTER_Y);
    game.setHero(newHero);
    game.setBase(newBase);
    game.run();

    return EXIT_SUCCESS;
}
