#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderProgram.h"
#include "Input.h"
#include "Camera.h"
#include "Triangle.h"

static void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods);

int constexpr WIDTH = 900;
int constexpr HEIGHT = 1600;

// struct Vertex {
//     float x, y, z;
//     float r, g, b;
// };

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
    glfwSetKeyCallback(window, key_callback);

    gladLoadGL();
    // std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // TODO: OPENGL STUFF HERE

    Input input(window);
    Camera cam = Camera();
    glm::vec3 camera_pos(0.0, 0.0, 1.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);
    float camera_speed = 0.05f;

    std::vector<struct Vertex> vertArr= {
        {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
        {0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}
    };

    Triangle triangle(vertArr);

    ShaderProgram shaders (
        "shaders/color.vert",
        "shaders/color.frag"
    );

    unsigned int vertBuff;
    glGenBuffers(1, &vertBuff);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);    
    // Assign buffer data
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), triangle.getVerts().data(), GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuff);  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid*) 0);

    // glBindBuffer(GL_ARRAY_BUFFER, colorId);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid*) offsetof(Vertex, r));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    input.bindKeyPress("quit", GLFW_KEY_ESCAPE, [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    while (!glfwWindowShouldClose(window)) {

        if(input.isKeyPressed(GLFW_KEY_S)) {
            camera_pos.y -= camera_speed;
        }
        if(input.isKeyPressed(GLFW_KEY_A)) {
            camera_pos.x -= camera_speed;
        }
        if(input.isKeyPressed(GLFW_KEY_D)) {
            camera_pos.x += camera_speed;
        }
        if(input.isKeyPressed(GLFW_KEY_Q)) {
            camera_pos.z += camera_speed;
        }
        if(input.isKeyPressed(GLFW_KEY_E)) {
            camera_pos.z -= camera_speed;
        }        

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        // Use for non-indexed buffers
        glDrawArrays(GL_TRIANGLES, 0, 3);


        
        // glDrawElements draws on an indexed buffer

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
}

static void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
