#version 330 core

uniform sampler2D texture_atlas;
in vec2 frag_uv;

out vec4 color_out;

void main() {
    color_out = texture(texture_atlas, frag_uv);
}
