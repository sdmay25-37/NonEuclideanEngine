#define NE_ENGINE_INCLUDE_ARCHIVE
#include "ne_engine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

ShaderProgram* shader_ptr;
HypRotate r_uniform_matrix = HypRotate(true);

// settings
constexpr unsigned int SCREEN_WIDTH = 1920;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

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

    HypTransform::poincare_to_hyperboloid(square_points);

    std::vector<glm::vec4> points_2d = square_points;
    // HypTransform::transform_3d_2d(points_2d);
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
			"../ne_engine/shaders/square_test.vert",
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

        points_2d = square_points;
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