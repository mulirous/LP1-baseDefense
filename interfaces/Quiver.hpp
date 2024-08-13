#pragma once
#include "../common.h"
#include "Item.hpp"

class Quiver : public Item
{
private:
    /// @brief Quiver arrow's number
    int arrows;

public:
    Quiver();
    Quiver(int arrows);
    int getArrows();
};