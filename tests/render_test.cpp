#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <entt/entt.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "JSONLoader.hpp"
#include "Render.hpp"
#include "ShaderProgram.hpp"
#include "Input.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

float rand_float() {
    return (float)rand() / RAND_MAX;
}

void sprite_update(entt::registry &registry, std::vector<glm::vec4> &uv_ranges, unsigned int UV_VBO) {
    auto view = registry.view<Sprite>();

    uv_ranges.clear();
    for(auto [entity, sprite]: view.each()) {
        uv_ranges.emplace_back(
            sprite.uv_min.x,
            sprite.uv_min.y,
            sprite.uv_max.x,
            sprite.uv_max.y
        );
    }

    glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data());
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    entt::registry registry;

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

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("res/tiles.png", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // build and compile our shader program
    ShaderProgram shaders(
			"shaders/sprite.vert",
			"shaders/sprite.frag"
	);

    std::srand(std::time(nullptr));

    int tile_width = width / 16;
    int tile_height = height / 16;

    float uv_width = 1.0 / tile_width;
    float uv_height = 1.0 / tile_height;

    int map_width = 20;
    int map_height = map_width * ASPECT_RATIO;

    float rect_size = 2.0 / (map_width - 1);

    std::vector<glm::mat4> model_mats;
    std::vector<glm::vec4> uv_ranges;

    int num_sprites = map_width * map_height;

    for(int i = 0; i < num_sprites; i++) {

        int x = i % map_width;
        int y = i / map_width;

        float rect_x = x * rect_size - 1.0;
        float rect_y = y * rect_size - 1.0;

        int tile_x = rand() % tile_width;
        int tile_y = rand() % tile_height;

        float u = tile_x * uv_width;
        float v = tile_y * uv_height;

        glm::vec3 position(rect_x, rect_y, 0.0);
        glm::vec3 scale(rect_size, rect_size, 1.0);
        glm::vec2 uv_min(u, v);
        glm::vec2 uv_max(u + uv_width, v + uv_height);

        glm::mat4 model_mat(1.0);
        model_mat = glm::translate(model_mat, position);
        model_mat = glm::scale(model_mat, scale);
        model_mats.push_back(model_mat);

        uv_ranges.emplace_back(uv_min.x, uv_min.y, uv_max.x, uv_max.y);

        const auto entity = registry.create();
        registry.emplace<Sprite>(entity, model_mat, uv_min, uv_max);
    }

    Render render_system;
    render_system.bind();

    shaders.bind();
    shaders.setUniform1i("texture_atlas", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    int count = 0;

    glm::vec3 camera_pos(0.0, 0.0, 1.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);
    float camera_speed = 0.05f;

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 proj_mat = glm::perspective(fov, ASPECT_RATIO, nearPlane, farPlane);

    Input charInput(window);
    JSONLoader loader("tests/bindings/example_bindings.json");

    auto bindings = loader.processFileArray();

    charInput.bindKeyPress("QUIT", [&window]() {
        glfwSetWindowShouldClose(window, true);
    });

    charInput.bindKeyPress("MOVE_UP", [&camera_pos, camera_speed]() {
        camera_pos.y += camera_speed;
    });

    charInput.bindKeyPress("MOVE_DOWN", [&camera_pos, camera_speed]() {
        camera_pos.y -= camera_speed;
    });

    charInput.bindKeyPress("MOVE_LEFT", [&camera_pos, camera_speed]() {
        camera_pos.x -= camera_speed;
    });

    charInput.bindKeyPress("MOVE_RIGHT", [&camera_pos, camera_speed]() {
        camera_pos.x += camera_speed;
    });

    charInput.bindKeyPress("ZOOM_IN", [&camera_pos, camera_speed]() {
        camera_pos.z += camera_speed;
    });

    charInput.bindKeyPress("ZOOM_OUT", [&camera_pos, camera_speed]() {
        camera_pos.z -= camera_speed;
    });

    charInput.bindContexts(bindings);

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        count++;
        if(count % 25 == 0) {
            uv_ranges.clear();

            auto view = registry.view<Sprite>();
            for(auto [entity, sprite] : view.each()) {
                int tile_x = rand() % tile_width;
                int tile_y = rand() % tile_height;

                float u = tile_x * uv_width;
                float v = tile_y * uv_height;

                sprite.uv_min = glm::vec2(u, v);
                sprite.uv_max = glm::vec2(u + uv_width, v + uv_height);
            }
        }

        glm::mat4 proj_view_mat = proj_mat * glm::lookAt(camera_pos, glm::vec3(camera_pos.x, camera_pos.y, 0.0), xyz(camera_up));
        shaders.setUniformMat4("proj_view_mat", proj_view_mat);

        render_system.render(registry);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	shaders.cleanup();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}