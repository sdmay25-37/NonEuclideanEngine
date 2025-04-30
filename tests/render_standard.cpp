
#include <iostream>

#include "App.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"

class WorldPlugin final : public Plugin {
public:
    void Build(App &app) override {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(LoadTextures).Then(CreateSprite)));
            // .AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    static void CreateSprite(entt::registry& registry, Resource<TextureManager> texture_manager) {
        std::srand(std::time(nullptr));
        auto texture_result = texture_manager->getTexture("cy.jpg");
        AtlasedTexture texture = texture_result.value();

        int map_size = 2;
        float rect_size = 0.5 / map_size;
        float total_size = rect_size * map_size;

        glm::vec3 position(0.0, 0.0, 0.0);
        glm::vec3 scale(rect_size, rect_size, 1.0);

        glm::mat4 model_mat(1.0);
        model_mat = glm::translate(model_mat, position);
        model_mat = glm::scale(model_mat, scale);

        const auto entity = registry.create();
        registry.emplace<AtlasSprite>(entity, 0, model_mat, texture);

        auto texture_result2 = texture_manager->getTexture("character.png");
        AtlasedTexture texture2 = texture_result2.value();

        const auto entity2 = registry.create();
        model_mat = glm::translate(model_mat, glm::vec3(0.25f, 0.0f, 0.0f));
        registry.emplace<AtlasSprite>(entity2, 1, model_mat, texture2);
    }

    static void LoadTextures(Resource<TextureManager> texture_manager) {
        texture_manager->LoadTextures("../res/textures");
    }

    static void MoveCamera(Resource<Camera> camera) {
        static long speed = 0;
        static float count = 0;

        count += std::abs(2.0 * std::sin(++speed / 50.0));

        float x = 0.5 * std::sin(count / 50.0);
        float y = 0.5 * std::cos(count / 50.0);

        camera->position = glm::vec3(x, y, 2.0f);
    }
};


int main() {

    glm::vec3 camera_pos(0.0, 0.0, 1.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;


    // glm::mat4 proj_mat = glm::perspective(fov, (800.0f / 600.0f), nearPlane, farPlane);

    int screen_width = 800;
    int screen_height = 600;

    float ortho_width = 1.0f;
    float ortho_height = ortho_width * ((float)screen_height / screen_width);

    glm::mat4 proj_mat = glm::ortho(-ortho_width, ortho_width, -ortho_height, ortho_height, nearPlane, farPlane);

    App()
        .InsertResourceBase<Window, GLFWWindow>(screen_width, screen_height)
        .AddPlugin<DefaultPlugins>()
        .AddPlugin<WorldPlugin>()
        .InsertResource<TextureManager>()
        .InsertResource<Camera>(camera_pos, camera_up, proj_mat)
        .Run();

    glfwTerminate();
    return 0;
}