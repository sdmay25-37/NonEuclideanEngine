#version 330 core

in vec4 frag_color;  // Optional per-vertex color
in vec2 frag_uv;

uniform sampler2D texture_atlas;

out vec4 FragColor;

void main() {
    vec4 tex_color = texture(texture_atlas, frag_uv);


    // Combine with vertex color (if desired), or just output texture
    FragColor = tex_color * frag_color;  // Tint texture with color
}
