#version 330 core

in vec3 frag_color;
in vec2 frag_uv;

uniform sampler2D img_texture;

out vec4 out_color;

void main() {
    //out_color = vec4(frag_color, 1.0);
    out_color = texture(img_texture, frag_uv);
}