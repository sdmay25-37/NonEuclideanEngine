//
// Created by benj2003 on 10/25/2024.
//

#ifndef SPRITE_H
#define SPRITE_H
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


class Sprite {
public:
    glm::vec3 position;
    glm::vec3 scale;

    Sprite(glm::vec3 position, glm::vec3 scale) {
        this->position = position;
        this->scale = scale;
    }
};



#endif //SPRITE_H
