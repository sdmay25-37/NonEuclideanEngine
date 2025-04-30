#version 330 core

// Per-instance uniforms
uniform mat4 proj_view_mat;
uniform mat4 r_matrix;


// Per-vertex attributes
layout(location = 0) in vec4 vert_position;// Position (x, y, z, w)
layout(location = 1) in vec4 vert_color;// Color (r, g, b, a)
layout(location = 2) in vec2 vert_uv;// UV coordinates for the texture (u, v)

// Per-instance attributes
layout(location = 3) in vec4 sprite_uv;
layout(location = 4) in float zindex;

// Outputs to fragment shader
out vec4 frag_color;
out vec2 frag_uv;

void main() {
    // Transform to world space
    vec4 position = vert_position;
    position = r_matrix * position;

    // Hyperbolic Weierstrass -> Poincaré disk projection
    position.x /= (1.0 + position.z);
    position.y /= (1.0 + position.z);
    position.z = -zindex;

    gl_Position = position;

    frag_color = vert_color;

    //vec2 uv_range = sprite_uv.zw - sprite_uv.xy;
    //frag_uv = sprite_uv.xy + vert_uv * uv_range;
    frag_uv = mix(sprite_uv.xy, sprite_uv.zw, vert_uv);
    // Clamp UV coordinates to ensure they are within the texture's bounds
    frag_uv = clamp(frag_uv, vec2(0.0), vec2(1.0));

}
