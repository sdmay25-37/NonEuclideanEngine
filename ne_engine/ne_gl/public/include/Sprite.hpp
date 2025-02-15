//
// Created by benj2003 on 10/25/2024.
//

#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


class Sprite {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec2 uv_min, uv_max;

    Sprite(glm::vec3 position, glm::vec3 scale, glm::vec2 uv_min, glm::vec2 uv_max) {
        this->position = position;
        this->scale = scale;
        this->uv_min = uv_min;
        this->uv_max = uv_max;
    }
};



#endif //SPRITE_H
