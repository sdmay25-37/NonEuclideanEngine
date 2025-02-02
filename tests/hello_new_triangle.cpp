#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderProgram.h"
#include "Input.h"
#include "Camera.h"
#include "Triangle.h"
#include "CharWrapper.h"

static void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods);
// static void moveLeftWrapper();

int constexpr WIDTH = 900;
int constexpr HEIGHT = 1600;

Triangle triangle;

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
    // glfwSetKeyCallback(window, key_callback);

    gladLoadGL();
    // std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // TODO: OPENGL STUFF HERE

    Camera cam = Camera();
    glm::vec3 camera_pos(0.0, 0.0, 1.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);
    float camera_speed = 0.05f;

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

    Input input(window);
    CharWrapper wrapper(triangle);

    input.bindKeyPress("QUIT", GLFW_KEY_ESCAPE, [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    input.bindKeyPress("MOVE_LEFT", GLFW_KEY_A, std::bind(&CharWrapper::moveLeftWrapper, &wrapper));
    input.bindKeyPress("MOVE_RIGHT", GLFW_KEY_D, std::bind(&CharWrapper::moveRightWrapper, &wrapper));
    input.bindKeyPress("MOVE_UP", GLFW_KEY_W, std::bind(&CharWrapper::moveUpWrapper, &wrapper));
    input.bindKeyPress("MOVE_Down", GLFW_KEY_S, std::bind(&CharWrapper::moveDownWrapper, &wrapper));

    while (!glfwWindowShouldClose(window)) {
                
        glfwWaitEvents();
        glBindBuffer(GL_ARRAY_BUFFER, vertBuff);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Triangle::Vertex), wrapper.getTriangle().getVerts().data(), GL_STATIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        // Use for non-indexed buffers
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // glDrawElements draws on an indexed buffer

        glfwSwapBuffers(window);

        // glfwPollEvents();
    }

    glfwTerminate();
}

// static void moveLeftWrapper() {
//     triangle.moveLeft();
// }


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
