#version 330 core

uniform mat4 r_matrix;
uniform vec3 color;

layout(location = 0) in vec4 vert_position;

out vec3 frag_color;

void main() {
    gl_Position = r_matrix * vert_position;
    frag_color = color;
}