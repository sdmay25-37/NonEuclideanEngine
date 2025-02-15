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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
constexpr unsigned int SCREEN_WIDTH = 1920;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

int main() {

    GLFWContext context = GLFWContext();
    context.initAll(SCREEN_WIDTH, SCREEN_HEIGHT, "Hyperboloid");

    // std::vector<Eigen::Vector3d> hyperboloid_verts;
    Square square(Square::SQUARE_XY);
    square = square * .25f;
    // std::vector<glm::vec3> square_points;
    // std::vector<unsigned int> square_indices({0, 1, 2, 3, 0});

    // #define SQUARES_PER_SIDE 1
    // #define POINT_DIST 1.0f / 2

    // glm::vec3 p0 = glm::vec3({-.5f, .5f, 0});
    // glm::vec3 p1 = glm::vec3({-.5f, -.5f, 0});
    // glm::vec3 p2 = glm::vec3({.5f, -.5f, 0});
    // glm::vec3 p3 = glm::vec3({.5f, .5f, 0});

    // square_points.push_back(p0);
    // square_points.push_back(p1);
    // square_points.push_back(p2);
    // square_points.push_back(p3);

    // build and compile our shader program
    ShaderProgram shaders(
			"shaders/vec4.vert",
			"shaders/color.frag"
	);

    shaders.bind();
    shaders.setUniform3f("color", glm::vec3(1, 0, 0));

    unsigned int VA0, VB0, VE0;

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    glGenBuffers(1, &VE0);

    glBindVertexArray(VA0);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * square.getPoints().size(), square.getPoints().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * square.getIndices().size(), square.getIndices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(context.getWindow())) {
        processInput(context.getWindow());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VA0);
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        glDrawElements(GL_LINE_STRIP, square.getIndices().size(), GL_UNSIGNED_INT, 0);

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