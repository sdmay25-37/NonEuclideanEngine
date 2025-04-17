#version 330 core

// Per-instance uniforms
uniform mat4 proj_view_mat;
uniform mat4 r_matrix;


// Per-vertex attributes
layout(location = 0) in vec4 vert_position;  // Position (x, y, z, w)
layout(location = 1) in vec4 vert_color;     // Color (r, g, b, a)
layout(location = 2) in vec2 vert_uv;        // UV coordinates for the texture (u, v)

// Per-instance attributes

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
    position.z = 0.0;

    gl_Position = position;

    frag_color = vert_color;

    // Map the texture coordinates (UV) to the fragment shader
    frag_uv = vert_uv;  // Pass the UV coordinates directly
}
