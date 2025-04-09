#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"

class WorldPlugin final : public Plugin
{
public:
    void Build(App &app) override
    {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles).After(LoadTextures)));
        //.AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    static void CreateTiles(entt::registry &registry, Resource<TextureManager> texture_manager)
    {
        std::srand(std::time(nullptr));
        auto texture_result = texture_manager->getTexture("floor2.jpg");
        AtlasedTexture texture = texture_result.value();

        auto texture_result2 = texture_manager->getTexture("wall.jpg");
        AtlasedTexture texture2 = texture_result2.value();

        int map_size = 10;
        float rect_size = 1.5 / map_size;
        float total_size = rect_size * map_size;

        int num_sprites = map_size * map_size;

        for (int i = 0; i < num_sprites; i++)
        {

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

            if (y % 2 == 0)
            {
                registry.emplace<AtlasSprite>(entity, model_mat, texture2);
            }
            else
            {
                registry.emplace<AtlasSprite>(entity, model_mat, texture);
            }
        }
    }

    static void LoadTextures(Resource<TextureManager> texture_manager)
    {
        texture_manager->LoadTextures("../res/textures");
    }

    static void MoveCamera(Resource<Camera> camera)
    {
        static float count = 0;
        static long speed = 0;

        count += std::abs(2.0 * std::sin(++speed / 50.0));

        float x = 0.5 * std::sin(count / 50.0);
        float y = 0.5 * std::cos(count / 50.0);

        camera->position = glm::vec3(x, y, 2.0f);
    }
};

int main()
{

    glm::vec3 camera_pos(0.0, 0.0, 2.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 proj_mat = glm::perspective(fov, (800.0f / 600.0f), nearPlane, farPlane);

    App()
        .InsertResourceBase<Window, GLFWWindow>(800, 600)
        .AddPlugin<DefaultPlugins>()
        .AddPlugin<WorldPlugin>()
        .InsertResource<TextureManager>()
        .InsertResource<Camera>(camera_pos, camera_up, proj_mat)
        .Run();

    glfwTerminate();
    return 0;
}
