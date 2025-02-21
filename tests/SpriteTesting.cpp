#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#include "ne_engine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

struct Vertex {
	float x, y, z;
    float u, v;
};

float rand_float() {
    return (float)rand() / RAND_MAX;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Input input(window);

    // build and compile our shader program
    ShaderProgram shaders(
			"../shaders/sprite.vert",
			"../shaders/sprite.frag"
	);

    // std::vector <Sprite> giratinaSprites;
    // 24 Frames

    std::srand(std::time(nullptr));


    std::vector<glm::mat4> model_mats;
    std::vector<glm::vec4> uv_ranges;

                                    // startRow, frameWidth, rowHeight, numFramesInSheet, numFramesInAnimation, looping
    struct AnimationData slimeData = {0,        1.0 / 33.0,  1.0 / 4.0, 33.0,             33.0, 1};

    glm::vec3 position(0.0, 0.0, 0.0);
    glm::vec3 scale(0.3, 0.3, 1.0);
    glm::vec2 uv_min(0.0, slimeData.startRow);
    glm::vec2 uv_max(slimeData.frameWidth, slimeData.startRow + slimeData.rowHeight);

    glm::mat4 model_mat(1.0);
    model_mat = glm::translate(model_mat, position);
    model_mat = glm::scale(model_mat, scale);
    model_mats.push_back(model_mat);

    Animation slime(position, scale, uv_min, uv_max,
        slimeData, "../res/Slime.png");

    slime.initAnimation();
    AnimationData checkInf = slime.getAnimationData();

    uv_ranges.emplace_back(0.0, slime.getAnimationData().startRow,
        slime.getAnimationData().frameWidth, slime.getAnimationData().startRow + slime.getAnimationData().rowHeight);    // 7 Frames

    std::vector<Vertex> vertices = {
        Vertex {  0.5,  0.5, 0.0, 1.0, 1.0 },
        Vertex {  0.5, -0.5, 0.0, 1.0, 0.0 },
        Vertex { -0.5, -0.5, 0.0, 0.0, 0.0 },
        Vertex { -0.5,  0.5, 0.0, 0.0, 1.0 }
    };

    std::vector<unsigned int> indices = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &UV_VBO);
    glGenBuffers(1, &MODEL_MAT_VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex texture coords attribute

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // sprite texture coords attribute

    glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // model matrix attribute

    glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_mats.size(), model_mats.data(), GL_DYNAMIC_DRAW);

    for(int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i);
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(3 + i, 1);
    }

    // index buffer

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);

    shaders.bind();
    shaders.setUniform1i("texture_atlas", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, slime.getTextureId());

    int count = 0;

    glm::vec3 camera_pos(0.0, 0.0, 1.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);
    float camera_speed = 0.05f;

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 proj_mat = glm::perspective(fov, ASPECT_RATIO, nearPlane, farPlane);

    input.bindKeyPress("quit", GLFW_KEY_ESCAPE, [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    int ctr = 0;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        uv_ranges.clear();
        uv_ranges.emplace_back(ctr * slime.getAnimationData().frameWidth, slime.getAnimationData().startRow, (ctr + 1.0) * (float) slime.getAnimationData().frameWidth, slime.getAnimationData().startRow + (slime.getAnimationData().rowHeight));

        glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data());

        glm::mat4 proj_view_mat = proj_mat * glm::lookAt(camera_pos, glm::vec3(camera_pos.x, camera_pos.y, 0.0), xyz(camera_up));
        shaders.setUniformMat4("proj_view_mat", proj_view_mat);

        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        ctr = (ctr + 1) % ((int) slime.getAnimationData().numFramesInAnimation);
    }

	shaders.cleanup();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &UV_VBO);
    glDeleteBuffers(1, &MODEL_MAT_VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void shiftCameraLeft() {

}