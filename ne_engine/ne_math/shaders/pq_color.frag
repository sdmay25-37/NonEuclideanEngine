#version 330 core

in vec4 frag_color;
in vec2 frag_uv;

uniform sampler2D img_texture;

out vec4 FragColor;

void main() {
    //FragColor = frag_color;
    FragColor = texture(img_texture, frag_uv);
}