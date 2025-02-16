#include "Animation.hpp"

Animation::Animation() 
: Sprite(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0)) {

}
   
Animation::Animation (glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max, int startRow, int endRow, int totalRows, int numFrames, int looping) 
    : Sprite (position, scale, uv_min, uv_max), _rows({startRow, endRow, totalRows}), _numFrames(numFrames), _loop(looping) {

}

// Figure out if i'm supposed to set up the Texture stuff in this part.    
void Animation::initAnimation() {

}