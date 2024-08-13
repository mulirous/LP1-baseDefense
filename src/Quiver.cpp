#include "../interfaces/Quiver.hpp"

Quiver::Quiver() : Item(QUIVER_IMAGE)
{
    arrows = 5;
}

Quiver::Quiver(int arrows) : Item(QUIVER_IMAGE)
{
    this->arrows = arrows;
}

int Quiver::getArrows()
{
    return this->arrows;
}