#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "WindowPlugin.hpp"
#include <glad/glad.h>

class WorldPlugin final : public Plugin {
public:
    void Build(App &app) override {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles).After(LoadTileTexture)))
            .AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));

    }

private:
    static void CreateTiles(entt::registry& registry, Resource<TextureManager> texture_manager) {
        std::srand(std::time(nullptr));
        auto texture_result = texture_manager->getTexture("cy.jpg");
        AtlasedTexture texture = texture_result.value();

        int map_size = 2;
        float rect_size = 0.5 / map_size;
        float total_size = rect_size * map_size;

        int num_sprites = map_size * map_size;

        for(int i = 0; i < num_sprites; i++) {

            int x = i % map_size;
            int y = i / map_size;

            float rect_x = x * rect_size - total_size / 2.0f + rect_size / 2.0f;
            float rect_y = y * rect_size - total_size / 2.0f + rect_size / 2.0f;

            glm::vec3 position(rect_x, rect_y, 0.0);
            glm::vec3 scale(rect_size, rect_size, 1.0);

            glm::mat4 model_mat(1.0);
            model_mat = glm::translate(model_mat, position);
            model_mat = glm::scale(model_mat, scale);

            const auto entity = registry.create();
            registry.emplace<AtlasSprite>(entity, model_mat, texture);
        }
    }

    static void LoadTileTexture(Resource<TextureManager> texture_manager) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char *data = stbi_load("../res/textures/tiles.png", &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        auto result = texture_manager->loadAtlas("../res/atlases/atlas.json");
        if(result.is_error()) {
            std::cerr << "Error: " << result.error();
        }

        // -- Better texture loading -- //
        texture_manager->LoadTextures("../res/textures");
    }

    static void MoveCamera(Resource<Camera> camera) {
        static float count = 0;
        static long speed = 0;

        count += std::abs(2.0 * std::sin(++speed / 50.0));

        float x = 0.5 * std::sin(count / 50.0);
        float y = 0.5 * std::cos(count / 50.0);

        camera->position = glm::vec3(x, y, 2.0f);
    }
};


int main() {

    glm::vec3 camera_pos(0.0, 0.0, 2.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 proj_mat = glm::perspective(fov, (800.0f / 600.0f), nearPlane, farPlane);

    App()
        .AddPlugin<WindowPlugin>()
        .AddPlugin<RenderPlugin>()
        .AddPlugin<WorldPlugin>()
        .InsertResource<TextureManager>()
        .InsertResource<Camera>(camera_pos, camera_up, proj_mat)
        .Run();

    glfwTerminate();
    return 0;
}
