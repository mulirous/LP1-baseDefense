#pragma once

#include "Item.hpp"
#include <memory>
#include <SFML/Graphics.hpp>
#include "../common.h"

class Potion : public Item
{
private:
    int healAmount;

public:
    Potion() : healAmount(10), Item(POTION_IMAGE) {};
    int getHealAmount() { return healAmount; }
};