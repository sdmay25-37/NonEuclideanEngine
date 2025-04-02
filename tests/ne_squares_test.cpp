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
    context.initAll(SCREEN_WIDTH, SCREEN_HEIGHT, "NE_Squares_Test");

    Square sq = Square(2.0f, 100);
    // sq.offset({1.0, 1.0, 0});
    sq.to_3d_hyperbolic();

    // HypTransform::poincare_to_hyperboloid(sq.get_verts());

    // build and compile our shader program
    ShaderProgram shaders(
			"../ne_engine/ne_math/shaders/square_test.vert",
			"../shaders/color.frag"
	);
    shader_ptr = &shaders;

    shaders.bind();
    shaders.setUniform3f("color", glm::vec3(1, 0, 0));
    shaders.setUniformMat4("r_matrix", r_uniform_matrix.getRotation());


    unsigned int VA0, VB0, VE0;

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    glGenBuffers(1, &VE0);

    glBindVertexArray(VA0);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sq.get_verts().size(), sq.get_verts().data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sq.get_indices().size(), sq.get_indices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(context.getWindow())) {
        processInput(context.getWindow());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VA0);
        glBindBuffer(GL_ARRAY_BUFFER, VB0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sq.get_verts().size(), sq.get_verts().data(), GL_DYNAMIC_DRAW);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, sq.get_indices().size(), GL_UNSIGNED_INT, 0);

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
        // r_uniform_matrix.offset({0, .1, 0, 1});
        // r_uniform_matrix.rotateZ(M_PI/16.0);
        // r_uniform_matrix.rotateZ(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateX(square_points, M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN))
    {
        r_uniform_matrix.rotateX(-M_PI / 16.0f);
        // r_uniform_matrix.offset({0,-.1, 0, 1});
        // r_uniform_matrix.getRotation()
        // glm::vec4 test_vec = r_uniform_matrix.getRotation() * glm::vec4({1, 0, 0, 1});

        // r_uniform_matrix.rotateZ(-M_PI / 16.0f);
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