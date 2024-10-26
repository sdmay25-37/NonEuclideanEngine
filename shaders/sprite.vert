#version 330 core

// Vertex specifc
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 vert_uv;

// Instance specific
layout(location = 2) in vec4 sprite_uv;
layout(location = 3) in vec4 model_mat_vecs[4];

out vec2 frag_uv;

void main() {
    mat4 model_mat = mat4(
        model_mat_vecs[0],
        model_mat_vecs[1],
        model_mat_vecs[2],
        model_mat_vecs[3]
    );

    gl_Position = model_mat * vec4(pos, 1.0);
    frag_uv = mix(sprite_uv.xy, sprite_uv.zw, vert_uv);
}