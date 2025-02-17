//
//  Author: tagrinnell
//  2/15/25
//

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Sprite.hpp"

// Struct for storing Row information
struct RowInf {
    int startRow;
    int endRow;
    int totalRows;
};

class Animation : public Sprite {
public:
    Animation ();
    Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, 
        int startRow, int endRow, int totalRows, int numFrames, int looping, const char *spritesheet);

    void initAnimation();

    // Getters
    unsigned int getTextureId();

    // Setters
    void setTextureId(unsigned int newTexture);

private:
    struct RowInf _rows;     // Row information (startRow, endRow, totalNums of rows)
    int _numFrames;          // Num Frames in the loop
    int _loop;               // If animation loops
    unsigned int _texture;   // Texture Buffer ID
    const char *_spritesheet;
    
};

// #endif // ANIMATION