#version 330 core

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_color;
layout(location = 2) in vec2 vert_uv;

out vec3 frag_color;
out vec2 frag_uv;

void main() {
    gl_Position = vec4(vert_position, 1.0);
    frag_color = vert_color;
    frag_uv = vert_uv;
}