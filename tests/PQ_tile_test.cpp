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
void processInput(GLFWwindow *window);

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
    PQTile tile = PQTile(4, 5, COLOR::BLUE);
    PQTile tile2 = PQTile(4, 5, COLOR::RED);
    PQTile tile3 = PQTile(4, 5, COLOR::GREEN);
    PQTile tile4 = PQTile(4, 5, COLOR::WHITE);

    // Convert tiles to Poincare representation
    tile.to_poincare();
    tile2.to_poincare();
    tile3.to_poincare();
    tile4.to_poincare();

    // Offset individual tiles via their mesh data
    for (auto &pt : tile2.mutable_mesh())
    {
        pt.x += 1.1f; // shift tile2 rightward
    }
    for (auto &pt : tile3.mutable_mesh())
    {
        pt.x -= 1.1f; // shift tile3 leftward
    }
    for (auto &pt : tile4.mutable_mesh())
    {
        pt.y += 1.1f; // shift tile4 upward
    }

    std::vector<PQTile> tiles = {tile}; //, tile2, tile3, tile4};

    // Build and compile our shader program
    auto shaderProgramResult = ShaderProgram::create(
        "../ne_engine/ne_math/shaders/pq_test.vert",
        "../ne_engine/ne_math/shaders/pq_color.frag");

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
    shader_ptr->setUniform1i("img_texture", 0);

    // Generate VAO, VBO, and EBO
    unsigned int VA0, VB0, VE0;
    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    glGenBuffers(1, &VE0);

    glBindVertexArray(VA0);
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);

    // // Setup vertex attribute pointers:
    // // Position attribute (location = 0): 4 floats, starting at offset 0
    // glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)0);
    // glEnableVertexAttribArray(0);
    // // Color attribute (location = 1): 4 floats, offset = 4 * sizeof(float)
    // glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)(4 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // // UV attribute (location = 2): 2 floats, offset = 8 * sizeof(float)
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)(8 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // Position (vec4)
    // Assuming 'Point' has a member 'color' of type 'Color'

    // Position (vec4)
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, x));
    glEnableVertexAttribArray(0);

    // Color (vec4) from the base class 'Point'
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, color));
    glEnableVertexAttribArray(1);

    // UV (vec2) in MeshPoint
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, uv));
    glEnableVertexAttribArray(2);

    // For debugging, you might uncomment the following to see wireframes
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the shader and texture each frame before drawing
        shaders.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Render each PQTile in our vector
        for (auto &currentTile : tiles)
        {

            currentTile.to_weirstrass();                              // Ensures Poincaré conversion
            auto &mesh = currentTile.mutable_mesh();                  // Vector of glm::vec3 (assuming this is mesh data)
            const unsigned int *indices = currentTile.indices_data(); // Assuming you have index data for triangle rendering

            glBindVertexArray(VA0);
            glBindBuffer(GL_ARRAY_BUFFER, VB0);
            glBufferData(GL_ARRAY_BUFFER, sizeof(MeshPoint) * currentTile.mesh_size(), currentTile.mesh_data(), GL_DYNAMIC_DRAW);

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

static int x = 0;
static int y = 0;
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP))
    {
        x += 1;
        r_uniform_matrix.rotateX(M_PI / 16.0f);
        std::cout << x << ", " << y << std::endl;
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN))
    {
        x -= 1;
        r_uniform_matrix.rotateX(-M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        y += 1;
        r_uniform_matrix.rotateY(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT))
    {
        y -= 1;
        r_uniform_matrix.rotateY(-M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
    4 else if (glfwGetKey(window, GLFW_KEY_R))
    {
        x = 0;
        y = 0;
        r_uniform_matrix = HypRotate(true);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
