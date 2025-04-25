#define NE_ENGINE_INCLUDE_ARCHIVE
#include "ne_engine.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderProgram.hpp"
#include <math.h>

#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
std::vector<PQTile> processInput(GLFWwindow *window, std::vector<PQTile> tiles);

ShaderProgram *shader_ptr;
HypRotate r_uniform_matrix = HypRotate(true);

// settings
constexpr unsigned int SCREEN_WIDTH = 1080;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

std::vector<glm::vec4> square_points;

int main()
{
    GLFWWindow test = GLFWWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
    GLFWwindow *window = static_cast<GLFWwindow *>(test.get());

    // Create PQTile objects (assuming PQTile inherits from Polygon and has a proper mutable_mesh() method)
    PQTile tile = PQTile(4, 5, COLOR::WHITE);
    PQTile tile2 = PQTile(4, 5, COLOR::BLUE);
    PQTile tile3 = PQTile(4, 5, COLOR::GREEN);
    PQTile tile4 = PQTile(4, 5, COLOR::RED);
    PQTile tile5 = PQTile(4, 5, COLOR::WHITE);

    // Convert tiles to Poincare representation
    tile.to_weirstrass();
    tile2.to_weirstrass();
    tile3.to_weirstrass();
    tile4.to_weirstrass();
    tile5.to_weirstrass();

    // tile2.translate(1.25, 0, 0);
    // tile3.translate(-1.25, 0, 0);
    // tile4.translate(0, 1.25, 0);
    // tile5.translate(0, 1.25, 0);

    tile2.rotateXHyperbolic(4.5 * M_PI / 16.0f);
    tile3.rotateYHyperbolic(-4.5 * M_PI / 16.0f);
    tile4.rotateXHyperbolic(4.5 * M_PI / 16.0f);
    tile4.rotateYHyperbolic(-4.5 * M_PI / 16.0f);
    tile5.rotateYHyperbolic(-4.5 * M_PI / 16.0f);
    tile5.rotateXHyperbolic(4.5 * M_PI / 16.0f);

    // Offset individual tiles via their mesh data

    std::vector<PQTile>
        tiles = {tile, tile2, tile3, tile4, tile5};

    // Build and compile our shader program
    auto shaderProgramResult = ShaderProgram::create(
        "../ne_engine/shaders/pq_test.vert",
        "../ne_engine/shaders/pq_color.frag");

    if (shaderProgramResult.is_error())
    {
        std::cerr << shaderProgramResult.error() << std::endl;
        return -1;
    }
    ShaderProgram shaders = shaderProgramResult.ok();
    shader_ptr = &shaders;

    shaders.bind();
    shaders.setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

    // Load a texture using stb_image
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, texChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("../res/textures/cy.jpg", &texWidth, &texHeight, &texChannels, 0);
    if (data)
    {
        GLenum format = (texChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // Set shader sampler uniform to texture unit 0 (if your fragment shader uses "img_texture")
    shaders.setUniform1i("texture_atlas", 0);

    // Generate VAO, VBO, and EBO
    unsigned int VA0, VB0, VE0, UV_VBO;
    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    glGenBuffers(1, &VE0);
    glGenBuffers(1, &UV_VBO);

    glBindVertexArray(VA0);
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * tile.data_size(), tile.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), tile.data_offset());
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Point), tile.color_offset());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // UV (vec2) in MeshPoint
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void *)offsetof(Point, uv));

    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Point), tile.uv_offset());
    glEnableVertexAttribArray(2);

    // glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);

    // glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(3);
    // glVertexAttribDivisor(3, 1);
    // For debugging, you might uncomment the following to see wireframes
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        tiles = processInput(window, tiles);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the shader and texture each frame before drawing
        shaders.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VA0);
        glBindBuffer(GL_ARRAY_BUFFER, VB0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * tile.data_size(), tile.data(), GL_DYNAMIC_DRAW);

        // Render each PQTile in our vector
        for (auto &currentTile : tiles)
        {

            currentTile.to_weirstrass(); // Ensures Poincaré conversion

            glBindVertexArray(VA0);
            glBindBuffer(GL_ARRAY_BUFFER, VB0);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * currentTile.data_size(), currentTile.data(), GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * currentTile.indices_size(), currentTile.indices_data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, currentTile.indices_size(), GL_UNSIGNED_INT, 0);
        }

        test.SwapBuffers();
        glfwPollEvents();
    }

    shaders.cleanup();
    glDeleteVertexArrays(1, &VA0);
    glDeleteBuffers(1, &VB0);
    glDeleteBuffers(1, &VE0);
    glDeleteTextures(1, &texture);

    glfwTerminate();
    return 0;
}

float Theta = M_PI / 3.0f;
std::vector<PQTile> processInput(GLFWwindow *window, std::vector<PQTile> tiles)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PQTile tile = PQTile(4, 5, COLOR::WHITE);
        PQTile tile2 = PQTile(4, 5, COLOR::BLUE);
        PQTile tile3 = PQTile(4, 5, COLOR::GREEN);
        PQTile tile4 = PQTile(4, 5, COLOR::RED);
        PQTile tile5 = PQTile(4, 5, COLOR::WHITE);

        Theta += 0.01;
        std::cout << "THETA:  " << Theta << "\n";
        // Convert tiles to Poincare representation
        tile.to_weirstrass();
        tile2.to_weirstrass();
        tile3.to_weirstrass();
        tile4.to_weirstrass();
        tile5.to_weirstrass();

        tile2.rotateXHyperbolic(Theta);
        tile3.rotateYHyperbolic(-Theta);
        tile4.rotateXHyperbolic(Theta);
        tile4.rotateYHyperbolic(-Theta);
        tile5.rotateYHyperbolic(-Theta);
        tile5.rotateXHyperbolic(Theta);

        // Offset individual tiles via their mesh data

        return {tile, tile2, tile3, tile4, tile5};
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PQTile tile = PQTile(4, 5, COLOR::WHITE);
        PQTile tile2 = PQTile(4, 5, COLOR::BLUE);
        PQTile tile3 = PQTile(4, 5, COLOR::GREEN);
        PQTile tile4 = PQTile(4, 5, COLOR::RED);
        PQTile tile5 = PQTile(4, 5, COLOR::WHITE);

        Theta -= 0.01;
        std::cout << "THETA:  " << Theta << "\n";
        // Convert tiles to Poincare representation
        tile.to_weirstrass();
        tile2.to_weirstrass();
        tile3.to_weirstrass();
        tile4.to_weirstrass();
        tile5.to_weirstrass();

        tile2.rotateXHyperbolic(Theta);
        tile3.rotateYHyperbolic(-Theta);
        tile4.rotateXHyperbolic(Theta);
        tile4.rotateYHyperbolic(-Theta);
        tile5.rotateYHyperbolic(-Theta);
        tile5.rotateXHyperbolic(Theta);
        return {tile, tile2, tile3, tile4, tile5};
        // x -= 1;
        // r_uniform_matrix.rotateX(-M_PI / 16.0f);
        // shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        // y += 1;
        // r_uniform_matrix.rotateY(M_PI / 16.0f);
        // shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT))
    {

        // y -= 1;
        // r_uniform_matrix.rotateY(-M_PI / 16.0f);
        // shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_R))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PQTile tile = PQTile(4, 5, COLOR::WHITE);
        PQTile tile2 = PQTile(4, 5, COLOR::BLUE);
        PQTile tile3 = PQTile(4, 5, COLOR::GREEN);
        PQTile tile4 = PQTile(4, 5, COLOR::RED);
        PQTile tile5 = PQTile(4, 5, COLOR::WHITE);
        Theta = M_PI / 3.0f;

        std::cout << "THETA:  " << Theta << "\n";
        // Convert tiles to Poincare representation
        tile.to_weirstrass();
        tile2.to_weirstrass();
        tile3.to_weirstrass();
        tile4.to_weirstrass();
        tile5.to_weirstrass();

        tile2.rotateXHyperbolic(Theta);
        tile3.rotateYHyperbolic(-Theta);
        tile4.rotateXHyperbolic(Theta);
        tile4.rotateYHyperbolic(-Theta);
        tile5.rotateYHyperbolic(-Theta);
        tile5.rotateXHyperbolic(Theta);

        // Offset individual tiles via their mesh data

        return {tile, tile2, tile3, tile4, tile5};
        // x = 0;
        // y = 0;
        // r_uniform_matrix = HypRotate(true);
        // shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    return tiles;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
