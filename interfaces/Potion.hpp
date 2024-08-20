#pragma once

#include "Item.hpp"
#include "../src/modules/animation/src/Animation.hpp"
#include <memory>
#include <SFML/Graphics.hpp>
#include "../common.h"

class Potion : public Item
{
private:
    int healAmount;
    std::shared_ptr<Animation> idleAnimation;

public:
    Potion();
    Potion(int healAmount);
    ~Potion() = default;
    int getHealAmount();
    void animate(float dt) override;
};