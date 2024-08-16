#pragma once

#include "Item.hpp"
#include "../src/modules/animation/src/Animation.hpp"

class ManaPotion : public Item
{
private:
    int manaAmount;
    std::shared_ptr<Animation> idleAnimation;

public:
    ManaPotion();
    ManaPotion(int mana);
    virtual ~ManaPotion() = default;
    int getMana();
    void animate(float dt) override;
};