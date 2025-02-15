// //
// //  Author: tagrinnell
// //  2/15/25
// //

// #ifndef SPRITERENDERER_H
// #define SPRITERENDERER_H

// #include "ShaderProgram.h"
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// class SpriteRenderer
// {
// public:
//     SpriteRenderer(ShaderProgram &shader);
//     ~SpriteRenderer();

//     void DrawSprite(Texture2D &texture, glm::vec2 position, 
//         glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, 
//         glm::vec3 color = glm::vec3(1.0f));
// private:
//     ShaderProgram shader; 
//     unsigned int quadVAO;

//     void initRenderData();
// };

// #endif // SPRITERENDERER_H