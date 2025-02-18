//
//  Author: tagrinnell
//  2/15/25
//

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <iostream>

#include "Sprite.hpp"

// Struct for storing Row information
struct AnimationData {
    int startRow;
    int totalRows;
    int numFrames;
    int animationFrames;
};

class Animation : public Sprite {
public:
    Animation ();
    Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, 
        int startRow, int totalRows, int numFrames, int animationFrames, int looping, const char *spritesheet);

    void initAnimation();

    // Getters
    unsigned int getTextureId() { return _textureId; };
    struct AnimationData getAnimationData() { return _animData; };

    // Setters
    void setTextureId(unsigned int newTexture);

private:
    struct AnimationData _animData;     // Row information (startRow, endRow, totalNums of rows)
    int _loop;               // If animation loops
    unsigned int _textureId;   // Texture Buffer ID
    const char *_spritesheet;
    
};

// #endif // ANIMATION