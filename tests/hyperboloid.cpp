#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Eigen/Eigen>

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "ne_engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

struct Vertex {
	float x, y, z;
    float u, v;
};
// settings
constexpr unsigned int SCREEN_WIDTH = 1920;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

int main() {

    GLFWContext context = GLFWContext();
    context.createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hyperboloid");
    context.setCurrent();
    context.initGLAD();

    // std::vector<Eigen::Vector3d> hyperboloid_verts;
    std::vector<glm::vec3> hyperboloid_verts;

    // hyperboloid_verts.push_back(glm::vec3(0,.5,0));
    // hyperboloid_verts.push_back(glm::vec3(.5, 0, 0));
    // hyperboloid_verts.push_back(glm::vec3(-.5,0,0));
    #define X_Z_OFFSET 10
    for(int i = -X_Z_OFFSET; i < X_Z_OFFSET + 1; i++)
    {
        for(int j = -X_Z_OFFSET; j < X_Z_OFFSET + 1;j++)
        {
            float x = i/(float)(X_Z_OFFSET*2);
            float z = j/(float)(X_Z_OFFSET*2);
            float y = sqrt(pow(x, 2) + pow(z, 2));

            hyperboloid_verts.push_back(glm::vec3(x, y, z));
        }
    }

    // build and compile our shader program
    ShaderProgram shaders(
			"shaders/color.vert",
			"shaders/color.frag"
	);

    shaders.bind();
    shaders.setUniform3f("color", glm::vec3(1, 0, 0));

    unsigned int VA0, VB0;

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);

    glBindVertexArray(VA0);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hyperboloid_verts.size(), hyperboloid_verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(context.getWindow())) {
        processInput(context.getWindow());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, hyperboloid_verts.size());

        context.swapBuffers();
        glfwPollEvents();
    }

	shaders.cleanup();
    glDeleteVertexArrays(1, &VA0);
    glDeleteBuffers(1, &VB0);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}