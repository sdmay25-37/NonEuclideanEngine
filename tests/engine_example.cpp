#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"

struct Speed {
    float dx, dy;
};

void LoadTextures(Resource<TextureManager> texture_manager) {
    texture_manager->LoadTextures("../res/textures");
}

void AddSprites(entt::registry& registry, Resource<TextureManager> texture_manager) {
    std::srand(std::time(nullptr));

    // Load texture
    auto texture_result = texture_manager->getTexture("cy.jpg");
    AtlasedTexture texture = texture_result.value();

    // Set position & size
    float size = 0.25f;
    glm::vec3 position(0.0f, 0.0f, 0.0f);
    glm::vec3 scale(size, size, 1.0f);

    // Create model matrix
    glm::mat4 model_mat(1.0);
    model_mat = glm::translate(model_mat, position);
    model_mat = glm::scale(model_mat, scale);

    const float max_speed = 0.02f;

    // Add sprites
    for(int i = 0; i < 100; i++) {
        // Initial speed
        float theta = (rand() / (float) RAND_MAX) * 2.0f * M_PI;
        float dx = max_speed * std::cos(theta);
        float dy = max_speed * std::sin(theta);
        Speed speed = {dx, dy};

        // Add components to entity
        const auto entity = registry.create();
        registry.emplace<AtlasSprite>(entity, model_mat, texture);
        registry.emplace<Speed>(entity, speed);
    }
}

void MoveSprites(entt::registry& registry) {
    auto view = registry.view<AtlasSprite, Speed>();
    for(auto [entity, sprite, speed] : view.each()) {
        glm::vec3 sprite_pos = sprite.model_mat[3];

        // Reflect on edge
        if(sprite_pos.x < -0.95f || sprite_pos.x > 0.95f) { speed.dx = -speed.dx; }
        if(sprite_pos.y < -0.70f || sprite_pos.y > 0.70f) { speed.dy = -speed.dy; }

        // Move sprite
        sprite.model_mat = glm::translate(sprite.model_mat, glm::vec3(speed.dx, speed.dy, 0.0f));
    }
}

int main() {
    App()
        // Add default plugins
        .AddPlugin<DefaultPlugins>()

        // Window and camera for rendering
        .InsertResourceBase<Window, GLFWWindow>(800, 600)
        .InsertResource<Camera>()

        // Texture manager
        .InsertResource<TextureManager>()

        // Load textures THEN create sprites
        .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(LoadTextures).Then(AddSprites)))

        // Move sprites on update
        .AddSystems(ScheduleLabel::UPDATE, SystemSet(MoveSprites))

        .Run();

    return 0;
}