#include "ne_engine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

ShaderProgram* shader_ptr;
HypRotate r_uniform_matrix = HypRotate(true);

// settings
constexpr unsigned int SCREEN_WIDTH = 1080;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

std::vector<glm::vec4> square_points;

int main() {

    GLFWContext context = GLFWContext();
    context.initAll(SCREEN_WIDTH, SCREEN_HEIGHT, "PQ_Tile_test");

    PQTile tile = PQTile(4, 5, COLOR::BLUE);

    tile.to_weirstrass();

    // build and compile our shader program
    ShaderProgram shaders(
			"../ne_engine/ne_math/shaders/pq_test.vert",
			"../ne_engine/ne_math/shaders/pq_color.frag"
	);

    shader_ptr = &shaders;

    shaders.bind();
    // shaders.setUniform3f("color", glm::vec3(1, 0, 0));
    shaders.setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

    unsigned int VA0, VB0, VE0;

    glGenVertexArrays(1, &VA0);
    glGenBuffers(1, &VB0);
    glGenBuffers(1, &VE0);

    glBindVertexArray(VA0);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VB0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshPoint) * tile.mesh_size(), tile.mesh_data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tile.indices_size(), tile.indices_data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(context.getWindow())) {
        processInput(context.getWindow());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VA0);
        glBindBuffer(GL_ARRAY_BUFFER, VB0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(MeshPoint) * tile.mesh_size(), tile.mesh_data(), GL_DYNAMIC_DRAW);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, tile.indices_size(), GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_POINTS, 0, tile.mesh_size());

        context.swapBuffers();
        glfwPollEvents();
    }

	shaders.cleanup();
    glDeleteVertexArrays(1, &VA0);
    glDeleteBuffers(1, &VB0);

    glfwTerminate();
    return 0;
}

static int x = 0;
static int y = 0;
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if(glfwGetKey(window, GLFW_KEY_UP))
    {
        x+=1;
        r_uniform_matrix.rotateX(M_PI / 16.0f);
        std::cout<< x << ", " << y << std::endl;
        // r_uniform_matrix.offset({0, .1, 0, 1});
        // r_uniform_matrix.rotateZ(M_PI/16.0);
        // r_uniform_matrix.rotateZ(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateX(square_points, M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN))
    {
        x-=1;
        std::cout<< x << ", " << y << std::endl;
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
        y+=1;
        std::cout<< x << ", " << y << std::endl;
        r_uniform_matrix.rotateY(M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateY(square_points, M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT))
    {
        y-=1;
        std::cout<< x << ", " << y << std::endl;
        r_uniform_matrix.rotateY(-M_PI / 16.0f);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
        // hyperbolic_rotateY(square_points, -M_PI / 8);
    }
    else if(glfwGetKey(window, GLFW_KEY_R))
    {
        x = 0;
        y = 0;
        r_uniform_matrix = HypRotate(true);
        shader_ptr->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}