#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#include "ne_engine.hpp"
#include "Triangle.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

struct Vertex {
	float x, y, z;
    float u, v;
};

int main() {
    // GLFW Init
    if (!glfwInit()) {
        std::cout << "GLFW Failed to initialize!" << std::endl;
    }

    // Create Window
    GLFWwindow* window = glfwCreateWindow(SCREEN_HEIGHT, SCREEN_WIDTH, "What tf is going on", NULL, NULL);
    // If fail
    if (!window) {
        std::cout << "WHAT DID YOU DO" << std::endl;
    }

    // Before using OpenGL, must make the window the current opengl context
    glfwMakeContextCurrent(window);

    gladLoadGL();

    // Set up a dummy triangle for triangling (testing)
    std::vector<Triangle::Vertex> vertArr;
    Triangle::Vertex vert0 = {-0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.0f};
    Triangle::Vertex vert1 = {0.0f, 0.05f, 0.0f, 0.0f, 1.0f, 0.0f};
    Triangle::Vertex vert2 = {0.05f, -0.05f, 0.0f, 0.0f, 0.0f, 1.0f};

    vertArr.push_back(vert0);
    vertArr.push_back(vert1);
    vertArr.push_back(vert2);

    Triangle newTriangle(vertArr, 0.05f);
    Input i;

    //
    //  Resource Manager Testing Portion
    //
    ResourceManager resManager; //("../tests/bindings/example_bindings.json");
    resManager.setLoaderPath("../tests/bindings/example_bindings.json");

    unsigned char* slimeArray = resManager.getResource("../res/Slime.png");
    unsigned char* newSlimeArray = resManager.getResource("../res/Slime.png");

    unsigned char* flip = resManager.getResource("../res/coinflip.png");

    resManager.removeReference("../res/Slime.png");
    resManager.removeReference("../res/Slime.png");
    resManager.removeReference("../res/coinflip.png");
    resManager.removeReference("../res/Slime.png");

    // End Testing

    Input input(window);
    
    auto bind = resManager.getBindingsArray();

    // Bind a ton of functions to the action string
    input.bindKeyPress("MOVE_LEFT", std::bind(&Triangle::moveLeft, &newTriangle));
    input.bindKeyPress("MOVE_RIGHT", std::bind(&Triangle::moveRight, &newTriangle));
    input.bindKeyPress("MOVE_UP", std::bind(&Triangle::moveUp, &newTriangle));
    input.bindKeyPress("MOVE_DOWN", std::bind(&Triangle::moveDown, &newTriangle));

    input.bindContexts(bind);


    ShaderProgram shaders (
        "../shaders/color.vert",
        "../shaders/color.frag"
    );

    unsigned vertBuff;
    glGenBuffers(1, &vertBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
    // Assign buffer data
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), newTriangle.getVerts().data(), GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    input.bindKeyPress("QUIT", [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    bool set = false;

    while (!glfwWindowShouldClose(window)) {

        glfwWaitEvents();
        glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), newTriangle.getVerts().data(), GL_STATIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        // Use for non-indexed buffers
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

    }

    glfwTerminate();
   
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void shiftCameraLeft() {

}