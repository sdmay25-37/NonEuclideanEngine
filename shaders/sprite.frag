#version 330 core

uniform sampler2D texture_atlas;
uniform vec2 uv;

in vec2 frag_uv;

out vec4 out_color;

void main() {
    out_color = texture(texture_atlas, frag_uv);
}