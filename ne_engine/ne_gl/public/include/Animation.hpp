//
//  Author: tagrinnell
//  2/15/25
//

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/glm.hpp>
#include <string>
#include <iostream>

#include "Sprite.hpp"

// Struct for storing Row information
struct AnimationData {
    int startRow;                       // The row the animation starts on
    float frameWidth;                   // Width of a frame in the animation
    float rowHeight;                    // The height of the row (e.g., sheet w/ 4 rows => rowHeight = 0.25)
    float numFramesInSheet;             // Total number of frames that can fit onto the sheet
    float numFramesInAnimation;         // Total number of frames for the specific animation
    int looping;
};

class Animation : public Sprite {
public:
    Animation ();
    // Constructor using the AnimationData struct
    Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, 
        struct AnimationData animationData, const char *spritesheet);
    // Constructor by parameter
    Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, 
        int startRow, float frameWidth, float rowHeight, float numFramesInSheet, float numFramesInAnimation, int looping, const char *spritesheet);

    void initAnimation();

    // Getters
    unsigned int getTextureId() { return _textureId; };
    struct AnimationData getAnimationData() { return _animData; };

    // Setters
    void setTextureId(unsigned int newTexture) { _textureId = newTexture; };
    struct AnimationData setAnimationData(struct AnimationData newData) { _animData = newData; };

private:
    struct AnimationData _animData;     // Row information (startRow, endRow, totalNums of rows)
    unsigned int _textureId;   // Texture Buffer ID
    const char *_spritesheet;
    
};

#endif // ANIMATION_HPP