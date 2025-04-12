#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"

#include "TileMap.hpp"
#include "Tile.hpp"
#include "Input.hpp"

class WorldPlugin final : public Plugin
{
public:
    void Build(App &app) override
    {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles).After(LoadTextures).After(LoadTiles).After(BindInput)))
            .AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    // used to render entire map
    static void CreateTiles(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        std::srand(std::time(nullptr));

        // map is 162 * 162 (image is 162 by 162)
        int map_size = 170;
        float rect_size = 10.0 / map_size;
        float total_size = rect_size * map_size;

        int num_sprites = map_size * map_size;

        std::cout << num_sprites << "\n";
        for (int i = 0; i < tilemap->numTiles; i++)
        {

            Tile tile = tilemap->getTileByID(i);
            // std::cout << tile.to_string() << "\n";
            int x = tile.worldPosition.first;
            int y = tile.worldPosition.second;
            // int x = i % map_size;
            // int y = i / map_size;

            float rect_x = x * rect_size - total_size / 2.0f + rect_size / 2.0f;
            float rect_y = y * rect_size - total_size / 2.0f + rect_size / 2.0f;

            glm::vec3 position(rect_x, rect_y, 0.0);
            glm::vec3 scale(rect_size, rect_size, 1.0);

            glm::mat4 model_mat(1.0);
            model_mat = glm::translate(model_mat, position);
            model_mat = glm::scale(model_mat, scale);

            const auto entity = registry.create();
            // tile.sprite is imagename.png, if imagename.png is not in res/textures might throw error
            // if tile.sprite is not specificed in json, uses cy.png
            auto texture_result = texture_manager->getTexture(tile.sprite);
            AtlasedTexture texture = texture_result.value();
            registry.emplace<AtlasSprite>(entity, model_mat, texture);
        }
    }

    // Used to render tiles in a specific distance from a tile
    static void CreateTiles2(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        std::srand(std::time(nullptr));

        Tile currentTile = tilemap->getTileByID(6400);

        std::vector<Tile> nearTiles = tilemap->getNearTiles(currentTile, 20);
        int map_size = 20;
        float rect_size = 1.5 / map_size;
        float total_size = rect_size * map_size;

        int num_sprites = map_size * map_size;

        std::cout << num_sprites << "\n";
        for (const Tile &tile : nearTiles)
        {
            // Tile tile = tilemap->getTileInRenderedList(i);
            //  std::cout << tile.to_string() << "\n";

            // Centers the rendering position so that the current tile is in middle of map
            int x = tile.relationMappingToCurrentTile.first + (map_size / 2);
            int y = tile.relationMappingToCurrentTile.second + (map_size / 2);
            // int x = i % map_size;
            // int y = i / map_size;

            float rect_x = x * rect_size - total_size / 2.0f + rect_size / 2.0f;
            float rect_y = y * rect_size - total_size / 2.0f + rect_size / 2.0f;

            glm::vec3 position(rect_x, rect_y, 0.0);
            glm::vec3 scale(rect_size, rect_size, 1.0);

            glm::mat4 model_mat(1.0);
            model_mat = glm::translate(model_mat, position);
            model_mat = glm::scale(model_mat, scale);

            const auto entity = registry.create();
            auto texture_result = texture_manager->getTexture(tile.sprite);
            AtlasedTexture texture = texture_result.value();
            registry.emplace<AtlasSprite>(entity, model_mat, texture);
        }
    }

    static void
    LoadTextures(Resource<TextureManager> texture_manager)
    {
        texture_manager->LoadTextures("../res/textures");
    }

    static void LoadTiles(Resource<TileMap> tileMap)
    {
        // Where Tiles are loaded from
        tileMap->loadTiles("../tests/json/maze_output.json");
    }

    static void MoveCamera(Resource<Camera> camera, Resource<Input> input)
    {
        const float speed = 0.02f;

        // Prioritize only one direction at a time
        if (input->isKeyPressed(GLFW_KEY_W))
        {
            camera->position.y += speed;
        }
        else if (input->isKeyPressed(GLFW_KEY_A))
        {
            camera->position.x -= speed;
        }
        else if (input->isKeyPressed(GLFW_KEY_S))
        {
            camera->position.y -= speed;
        }
        else if (input->isKeyPressed(GLFW_KEY_D))
        {
            camera->position.x += speed;
        }
    }

    static void BindInput(Resource<Input> input, Resource<Window> window)
    {
        auto glfw_window = static_cast<GLFWwindow *>(window->get()); // Using get() to access GLFWwindow*
        input->BindWindow(glfw_window);
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
        .InsertResource<TileMap>()
        .InsertResource<Camera>(camera_pos, camera_up, proj_mat)
        .InsertResource<Input>()
        .Run();

    glfwTerminate();
    return 0;
}
