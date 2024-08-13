#include "../interfaces/Potion.hpp"

Potion::Potion() : Item(POTION_IMAGE)
{
    healAmount = 10;
}

Potion::Potion(int healAmount) : Item(POTION_IMAGE)
{
    this->healAmount = healAmount;
}

int Potion::getHealAmount() { return healAmount; }