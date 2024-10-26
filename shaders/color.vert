#version 330 core

uniform vec3 color;

layout(location = 0) in vec3 vert_position;

out vec3 frag_color;

void main() {
    gl_Position = vec4(vert_position, 1.0);
    frag_color = color;
}