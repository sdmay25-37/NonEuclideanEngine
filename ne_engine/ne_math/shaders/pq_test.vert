#version 330 core

uniform mat4 r_matrix;

layout(location = 0) in vec4 vert_position;
layout(location = 1) in vec4 vert_color;
layout(location = 2) in vec2 vert_uv; // New texture coordinate attribute

out vec4 frag_color;
out vec2 frag_uv;              // Pass UV to fragment shader

void main() {
    vec4 position = vert_position;
    position = r_matrix * position;

    // Weirstrass/Hyperboloid to Poincare transformation
    position.x = position.x / (1 + position.z);
    position.y = position.y / (1 + position.z);
    position.z = 0;

    gl_Position = position;
    frag_color = vert_color;
    frag_uv = vert_uv;  // Send UV coordinates along
}
