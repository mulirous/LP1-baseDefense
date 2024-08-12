#pragma once
#include "../common.h"
#include "Item.hpp"

class Quiver : public Item
{
private:
    /// @brief Quiver arrow's number
    int arrows;

public:
    Quiver(int arrows) : Item(QUIVER_IMAGE) { this->arrows = arrows; }
    int getArrows() { return this->arrows; }
};