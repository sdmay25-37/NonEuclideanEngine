#include "Animation.hpp"

Animation::Animation() 
: Sprite(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0)) {

}
   
Animation::Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, int startRow, int totalRows, int numFrames, int animationFrames, int looping, const char *spritesheet) 
    : Sprite (position, scale, uv_min, uv_max), _animData({startRow, totalRows, numFrames, animationFrames}), _loop(looping), _spritesheet(spritesheet) {

}

// Figure out if i'm supposed to set up the Texture stuff in this part.    
void Animation::initAnimation() {
    // Set up Textures
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load Spritesheet
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(_spritesheet, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}