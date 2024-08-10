#pragma once
#include "Item.hpp"
#include <memory>
#include <SFML/Graphics.hpp>
#include "common.h"
#include <iostream>

class Potion : public Item
{
private:
    int healAmount;

public:
    Potion() : healAmount(10) {};
    int getHealAmount() { return healAmount; }
};