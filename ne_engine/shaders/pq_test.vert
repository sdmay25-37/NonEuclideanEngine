#version 330 core

uniform mat4 r_matrix;

layout(location = 0) in vec4 vert_position;
layout(location = 1) in vec4 vert_color;

out vec4 frag_color;

void main() {
    vec4 position = vert_position;
    // position.z = position.z - 1;

    position = r_matrix * position;

    // Weirstress/Hyperboloid to Poincare
    position.x = position.x / (1 + position.z);
    position.y = position.y / (1 + position.z);
    position.z = 0;

    gl_Position = position;
    frag_color = vert_color;
}