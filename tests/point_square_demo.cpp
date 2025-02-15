#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "ne_engine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

ShaderProgram* shader_ptr;
HypRotate r_uniform_matrix = HypRotate(true);

// settings
constexpr unsigned int SCREEN_WIDTH = 1920;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

void hyperbolic_transform(std::vector<glm::vec4>& points)
{
    // const glm::vec4 FOCAL_POINT = {0, 0, 0, 0};
    // const float PLANE_Z = -1;

    for(int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = std::sqrt( std::pow(x, 2) + std::pow(y, 2));

        points[i] = {x, y, z, 1.0f};


        glm::vec3 point = {x, y, z};
    }
}

void hyperbolic_rotateX(std::vector<glm::vec4>& points, float theta)
{
    HypRotate r_matrix = HypRotate(true);
    r_matrix.rotateX(theta);

    r_uniform_matrix.rotateX(theta);

    for(int i = 0; i < points.size();i++)
    {
        // points[i] = r_matrix * points[i];
        points[i] = r_uniform_matrix * points[i];
    }
}

void hyperbolic_rotateY(std::vector<glm::vec4>& points, float theta)
{
    HypRotate r_matrix = HypRotate(true);
    r_matrix.rotateY(theta);

    r_uniform_matrix.rotateY(theta);

    for(int i = 0; i < points.size(); i++)
    {
        // points[i] = r_matrix * points[i];
        points[i] = r_uniform_matrix * points[i];
    }
}

std::vector<glm::vec4> transform_2d(const std::vector<glm::vec4>& points)
{
    std::vector<glm::vec4> points_2d;
    points_2d.reserve(points.size());
    for(int i = 0; i < points.size(); i++)
    {
        points_2d.push_back({0, 0, 0, 1});
        points_2d[i].x = points[i].x ;/// (points[i].z + 1);
        points_2d[i].y = points[i].y ;/// (points[i].z + 1);
    }

    return points_2d;
}

std::vector<glm::vec4> square_points;

int main() {

    GLFWContext context = GLFWContext();
    context.initAll(SCREEN_WIDTH, SCREEN_HEIGHT, "Hyperboloid");

    // std::vector<Eigen::Vector3d> hyperboloid_verts;
    // Square square(Square::SQUARE_XY);
    // square = square * .25f;
    #define ROOT_NUM_POINTS 1000
    #define NUM_POINTS ROOT_NUM_POINTS * ROOT_NUM_POINTS / 4

    // for(int i = 0; i < 21; i++)
    // {
    //     for(int j = 0; j < 1001; j++)
    //     {
    //         square_points.push_back({-1.0f + i/10.0f,-1.0f + j/500.0f, 0, 1.0f});
    //     }
    // }
    // for(int i = 0; i < 21; i++)
    // {
    //     for(int j = 0; j < 1001; j++)
    //     {
    //         square_points.push_back({-1.0f + j/500.0f,-1.0f + i/10.0f, 0, 1.0f});
    //     }
    // }
    // for(int i = 0; i < NUM_POINTS; i++)
    // {
    //     square_points.push_back({.5f, (-1.0f + i/500000.0f), 0.0f, 1.0f});
    // }
    for(int i = 0; i < ROOT_NUM_POINTS/2; i++)
    {
        float y = 0.25f - (i / (float)ROOT_NUM_POINTS);
        for(int j = 0; j < ROOT_NUM_POINTS/2; j++)
        {
            float x = -0.25f + (j / (float)ROOT_NUM_POINTS);

            glm::vec4 point = glm::vec4({x, y, 0, 1});
            square_points.push_back(point);
        }
    }

    hyperbolic_transform(square_points);

    std::vector<glm::vec4> points_2d = transform_2d(square_points);
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
			"../shaders/vec4.vert",
			"../shaders/color.frag"
	);
    shader_ptr = &shaders;

    shaders.bind();
    shaders.setUniform3f("color", glm::vec3(1, 0, 0));
    shaders.setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

    unsigned int VA0, VB0, VE0;

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    // glGenBuffers(1, &VE0);

    glBindVertexArray(VA0);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * points_2d.size(), points_2d.data(), GL_DYNAMIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * square.getIndices().size(), square.getIndices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(context.getWindow())) {
        processInput(context.getWindow());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        points_2d = transform_2d(square_points);
        glBindVertexArray(VA0);
        glBindBuffer(GL_ARRAY_BUFFER, VB0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * square_points.size(), square_points.data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_POINTS, 0, points_2d.size());
        // glDrawElements(GL_LINE_STRIP, square.getIndices().size(), GL_UNSIGNED_INT, 0);

        context.swapBuffers();
        glfwPollEvents();
    }

	shaders.cleanup();
    glDeleteVertexArrays(1, &VA0);
    glDeleteBuffers(1, &VB0);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP))
    {
        r_uniform_matrix.rotateX(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateX(square_points, M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN))
    {
        r_uniform_matrix.rotateX(-M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateX(square_points, -M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        r_uniform_matrix.rotateY(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateY(square_points, M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT))
    {
        r_uniform_matrix.rotateY(-M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateY(square_points, -M_PI / 8);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}