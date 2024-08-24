#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../../../enums/CharacterDirection.h"

/// @file Animation.hpp
/// @class Animation
/// @brief Class representing an animation using a spritesheet.
/// Handles the animation of a sprite by managing the frames and the timing for switching between frames.
/// It supports both flipping the animation based on direction and updating the current frame based on elapsed time.
class Animation
{
private:
    /// @brief Number of textures in each row of the spritesheet.
    /// @details Represents the number of frames per row in the spritesheet. This information is used to
    /// calculate the position of each frame within the texture.
    sf::Vector2u imageCount;

    /// @brief Current frame index in the spritesheet.
    /// @details Keeps track of the current frame's position in the animation. It is used to determine which
    /// frame to display based on the elapsed time.
    sf::Vector2u current;

    /// @brief Total elapsed time since the last frame switch.
    /// @details Used to accumulate time and determine when to switch to the next frame.
    float totalTime;

    /// @brief Time between frame switches.
    /// @details Determines how long each frame is displayed before switching to the next one. This value affects
    /// the speed of the animation.
    float switchTime;

    /// @brief Vector of frame sizes and positions.
    /// @details Stores the sizes and positions of each frame in the spritesheet. If provided, it overrides
    /// the default frame calculation.
    std::unique_ptr<std::vector<sf::IntRect>> framesSizes;

public:
    /// @brief Constructor for the Animation class.
    ///
    /// Initializes the animation with a spritesheet texture, number of frames, and timing information.
    /// If `framesSizes` is provided, it is used to set the frames directly; otherwise, frames are
    /// calculated based on the texture size and number of frames, so will have the same width/height.
    /// @param texture Raw pointer to the texture used for the animation.
    /// @param imageCount Number of frames in the spritesheet row and column.
    /// @param switchTime Time between frame switches in seconds.
    /// @param framesSizes Optional parameter to specify the exact frames. If not provided, frames are calculated.
    /// @note Texture is a raw parameter in order to achieve compatibility with `ResourceManager` lib.
    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime, std::unique_ptr<std::vector<sf::IntRect>> framesSizes = nullptr);

    /// @brief Default Animation destructor.
    virtual ~Animation() = default;

    /// @brief Rectangle representing the current frame's size and position in the spritesheet.
    /// Used to set the texture rect of the sprite to display the correct frame.
    sf::IntRect textureRect;

    /// @brief Update the animation based on elapsed time.
    /// @details Advances the animation by one frame if enough time has passed. It updates the `textureRect` to
    /// display the current frame.
    /// @param dt Elapsed time since the last update in seconds.
    /// @note On our current context we have only one row and 'n' columns on spritesheets. So if you need a spritesheet
    /// with more than one row, just adapt this method to receive a 'row' parameter (or something like that).
    void update(float dt);

    /// @brief Update the animation based on elapsed time and character direction.
    /// @details Advances the animation by one frame and flips the frame horizontally if the character is facing left.
    /// This method is used for animations where the direction affects the appearance.
    /// @param dt Elapsed time since the last update in seconds.
    /// @param direction Direction in which the character is facing.
    void update(float dt, CharacterDirection direction);

    /// @brief Check if the animation has completed.
    /// @return True if the animation has completed all frames or false otherwise.
    bool isCompleted() const;

    /// @brief Reset the animation to the first frame.
    /// Sets the current frame index back to the start of the animation, allowing it to be played from the beginning.
    void reset();
};