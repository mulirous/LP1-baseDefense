#pragma once

#include <SFML/Graphics.hpp>
#include "Character.hpp"
#include "Quiver.hpp"
#include "../src/modules/animation/src/AnimationManager.hpp"
#include "../src/modules/texture_manager/src/ResourceManager.hpp"
#include <memory>
#include "Item.hpp"
#include "RangedWeapon.hpp"
#include <iostream>
#include "Potion.hpp"
#include <type_traits>

/// @brief A class that represents the main character, known as "Hero"
class Hero : public Character
{
protected:
    /// @brief Pointer to hero's ranged weapon
    std::shared_ptr<RangedWeapon> weapon;
    /// @brief Position that hero will move to
    sf::Vector2f targetPosition;
    /// @brief Hero's heal action
    /// @param healAmount Amount of health
    /// @note If the hero's life exceeds 100, no effect occurs
    void heal(int healAmount);
    void recharge(int ammo);
    void updateAnimation(const std::string &action, float dt);

public:
    Hero(float width, float height, float speed, int maxLife, float posX, float posY);
    void setTargetPosition(sf::Vector2f target);
    std::shared_ptr<RangedWeapon> getRangedWeapon();
    void takeDamage(int damage);
    void initAnimations() override;
    void move(float deltaTime) override;
    void doAttack(sf::Vector2f &target) override;

    template <typename T>
    void useItem(std::shared_ptr<T> item)
    {
        static_assert(std::is_base_of<Item, T>::value, "Not an item.");
        try
        {
            auto potion = std::dynamic_pointer_cast<Potion>(item);
            if (potion)
            {
                int life = potion->getHealAmount();
                this->heal(life);
                return;
            }
            auto quiver = std::dynamic_pointer_cast<Quiver>(item);
            if (quiver)
            {
                std::cout << "Hero is getting ammo from quiver\n";
                int ammo = quiver->getArrows();
                this->recharge(ammo);
                return;
            }
        }
        catch (std::exception &ex)
        {
            return;
        }
    }
};
