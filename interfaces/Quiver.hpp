#pragma once
#include "../common.h"
#include "Item.hpp"

class Quiver : public Item
{
private:
    int arrows;

public:
    Quiver();
    Quiver(int arrows);
    int getArrows();
    void animate(float dt) override;
};