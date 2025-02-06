#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderProgram.h"
#include "Input.h"
#include "Triangle.h"
#include "CharWrapper.h"
#include "JSONLoader.h"

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

    // TODO: OPENGL STUFF HERE

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
    // charInput = newcharInput;
    CharWrapper wrapper(triangle);
    JSONLoader loading("../ne_engine/public/bindings/example_bindings.json");

    std::vector<std::vector <std::pair <std::string, int>>> bindings = loading.processFileArray();

    charInput.bindKeyPress("QUIT", [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    // charInput.bindKeyPress("MOVE_LEFT", GLFW_KEY_A, std::bind(&CharWrapper::moveLeftWrapper, &wrapper));
    // charInput.bindKeyPress("MOVE_RIGHT", GLFW_KEY_D, std::bind(&CharWrapper::moveRightWrapper, &wrapper));
    // charInput.bindKeyPress("MOVE_UP", GLFW_KEY_W, std::bind(&CharWrapper::moveUpWrapper, &wrapper));
    // charInput.bindKeyPress("MOVE_DOWN", GLFW_KEY_S, std::bind(&CharWrapper::moveDownWrapper, &wrapper));

    charInput.bindKeyPress("MOVE_LEFT", std::bind(&CharWrapper::moveLeftWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_RIGHT", std::bind(&CharWrapper::moveRightWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_UP", std::bind(&CharWrapper::moveUpWrapper, &wrapper));
    charInput.bindKeyPress("MOVE_DOWN", std::bind(&CharWrapper::moveDownWrapper, &wrapper));
    charInput.bindKeyPress("SWITCH", std::bind(&Input::switchBindings, &charInput));

    charInput.bindContexts(bindings);
    // for (int i = 0; i < bindings.size(); i++) {
    //     charInput.bindKeyPress(bindings.at(i).first, bindings.at(i).second);
    // }


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

    // loading.outputBindings(charInput.getBindings());

    glfwTerminate();
}

static void bindWrap() {
    // charInput.bindKeyPress("MOVE_LEFT", GLFW_KEY_Q);
}


static void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case (GLFW_KEY_ESCAPE) :
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case (GLFW_KEY_W) :
                std::cout << "HIT W" << std::endl;
                // triangle.moveUp();
                break;
            case (GLFW_KEY_S) :
                std::cout << "HIT S" << std::endl;
                // triangle.moveDown();
                break;
            case (GLFW_KEY_A) :
                std::cout << "HIT A" << std::endl;
                // triangle.moveLeft();
                break;
            case (GLFW_KEY_D) :
                std::cout << "HIT D" << std::endl;
                // triangle.moveRight();
                break;
            default :
                break;
        }
    } 

    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), triangle.getVerts().data(), GL_STATIC_DRAW);
}
