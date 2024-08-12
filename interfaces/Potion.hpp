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
    Potion();
    Potion(int healAmount);
    ~Potion() = default;
    int getHealAmount();
};