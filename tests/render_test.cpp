#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "App.hpp"

class WorldPlugin {
    public:
    static void CreateTiles(entt::registry& registry, Resource<TextureManager> texture_manager) {
        std::srand(std::time(nullptr));
        auto texture_result = texture_manager->getTexture("tile0.png");
        AtlasedTexture texture = texture_result.value();

        int map_width = 20;
        int map_height = map_width * ASPECT_RATIO;

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
};



int main() {

    App()
        .AddSystems(ScheduleLabel::STARTUP, SystemSet(WorldPlugin::CreateTiles))
        .InsertResource<TextureManager>()
        .run();


    glfwTerminate();
    return 0;
}
