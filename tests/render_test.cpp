#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "WindowPlugin.hpp"
#include <glad/glad.h>

class WorldPlugin final : public Plugin {
public:
    void Build(App &app) override {
        app.AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles).After(LoadTileTexture)));
    }

private:
    static void CreateTiles(entt::registry& registry, Resource<TextureManager> texture_manager) {
        std::srand(std::time(nullptr));
        auto texture_result = texture_manager->getTexture("tile0.png");
        AtlasedTexture texture = texture_result.value();

        int map_width = 20;
        int map_height = map_width * (800.0f / 600.0f);

        float rect_size = 2.0 / (map_width - 1);
        int num_sprites = map_width * map_height;

        for(int i = 0; i < num_sprites; i++) {

            int x = i % map_width;
            int y = i / map_width;

            float rect_x = x * rect_size - 1.0;
            float rect_y = y * rect_size - 1.0;


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
    }
};


int main() {

    App()
        .AddPlugin<WindowPlugin>()
        .AddPlugin<RenderPlugin>()
        .AddPlugin<WorldPlugin>()
        .InsertResource<TextureManager>()
        .Run();

    glfwTerminate();
    return 0;
}
