#pragma once

#include <string>
#include <random>

const float BORDER_MARGIN = 35.f;
const int GAME_WINDOW_HEIGHT = 800;
const int GAME_WINDOW_WIDTH = 1200;
const int PROJECTILE_VELOCITY = 250;
const int HERO_VELOCITY = 150;
const float DROP_EXPIRATION_SECONDS = 7;
const std::string ASSETS_FOLDER = "resources/assets/textures";
const std::string AUDIO_FOLDER = "resources/assets/audio";
const std::string GAME_FONT = "resources/fonts/AGoblinAppears-o2aV.ttf";
const std::string MENU_IMAGE = "resources/assets/textures/menu.png";
const std::string BAD_GAMEOVER_IMAGE = "resources/assets/textures/bad-gameover.jpg";
const std::string BACKGROUND_GAME = "resources/assets/textures/darkforestbg.png";
const std::string BASE_IMAGE = "resources/assets/textures/darkcircle.png";
const std::string HERO_WALK_IMAGE = "resources/assets/textures/hero/Wizard-Walk.png";
const std::string HERO_IDLE_IMAGE = "resources/assets/textures/hero/Wizard-Idle.png";
const std::string HERO_ATTACK_IMAGE = "resources/assets/textures/hero/Wizard-Attack02.png";
const std::string POTION_IMAGE = "resources/assets/textures/items/red-potion.png";
const std::string BLUE_POTION_IMAGE = "resources/assets/textures/items/blue-potion.png";
const std::string ENEMY_WALK_IMAGE = "resources/assets/textures/enemy/Skeleton-Walk.png";
const std::string ENEMY_ATTACK_IMAGE = "resources/assets/textures/enemy/Skeleton-Attack.png";
const std::string QUIVER_IMAGE = "resources/assets/textures/items/quiver.png";
const std::string MENU_MUSIC = "resources/assets/audio/menu-soundtrack.ogg";
const std::string GAMEOVER_MUSIC = "resources/assets/audio/gameover-soundtrack.wav";
const std::string BATTLE_MUSIC = "resources/assets/audio/battle-soundtrack.ogg";
const std::string SPELL_MUSIC = "resources/assets/audio/spell.wav";
const std::string ARROW_MUSIC = "resources/assets/audio/arrowhit.wav";
const std::string WIZARD_SPELL = "resources/assets/textures/items/wizard_spell.png";
const std::string ENEMY_ARROW = "resources/assets/textures/items/enemy_arrow.png";
const std::string DRINKING_SOUND = "resources/assets/audio/drinking_potion.ogg";

inline int getRandomNumber(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);

    return dist(gen);
}
