#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderProgram.hpp"
#include "Input.hpp"
#include "Triangle.hpp"
#include "CharWrapper.hpp"
#include "JSONLoader.hpp"

static void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods);
static void bindWrap();

int constexpr WIDTH = 900;
int constexpr HEIGHT = 1600;

Triangle triangle;
// Input charInput;

unsigned int vertBuff;

int main() {
    // GLFW Init
    if (!glfwInit()) {
        std::cout << "GLFW Failed to initialize!" << std::endl;
    }

    // Create Window
    GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "What tf is going on", NULL, NULL);
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
    triangle = newTriangle;

    ShaderProgram shaders (
        "../shaders/color.vert",
        "../shaders/color.frag"
    );

    glGenBuffers(1, &vertBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
    // Assign buffer data
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), triangle.getVerts().data(), GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle::Vertex), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Input charInput(window);
    CharWrapper wrapper(triangle);
    JSONLoader loading("../ne_engine/public/bindings/example_bindings.json");

    // Process a json array of bindings that has multiple contexts
    // --> see ne_engine/public/bindings/example_bindings.json for the example
    std::vector<std::vector <std::pair <std::string, int>>> bindings = loading.processFileArray();

    charInput.bindKeyPress("QUIT", [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    // Bind a ton of functions to the action string
    charInput.bindKeyPress("MOVE_LEFT", std::bind(&CharWrapper::moveLeftWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_RIGHT", std::bind(&CharWrapper::moveRightWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_UP", std::bind(&CharWrapper::moveUpWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_DOWN", std::bind(&CharWrapper::moveDownWrapper, &wrapper));
    charInput.bindKeyPress("SWITCH", std::bind(&Input::switchBindings, &charInput));

    // Bind keys to the action strings through a list of list of pairs <string -> key>
    charInput.bindContexts(bindings);

    bool set = false;

    while (!glfwWindowShouldClose(window)) {

        glfwWaitEvents();
        glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), wrapper.getTriangle().getVerts().data(), GL_STATIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        // Use for non-indexed buffers
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

    }

    loading.outputBindingsArray(charInput.getBindingsArray());

    glfwTerminate();
}