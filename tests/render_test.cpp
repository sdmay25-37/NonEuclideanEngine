#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"

#include "TileMap.hpp"
#include "Tile.hpp"

class WorldPlugin final : public Plugin
{
public:
    void Build(App &app) override
    {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles2).After(LoadTextures).After(LoadTiles)));
        //.AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    static void CreateTiles(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        std::srand(std::time(nullptr));
        int map_size = 170;
        float rect_size = 1.5 / map_size;
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
            auto texture_result = texture_manager->getTexture(tile.sprite);
            AtlasedTexture texture = texture_result.value();
            registry.emplace<AtlasSprite>(entity, model_mat, texture);
        }
    }

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

            // TODO get bottom left most tile in nearTiles
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
        tileMap->loadTiles("../tests/json/maze_output.json");
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
        .InsertResource<TileMap>()
        .InsertResource<Camera>(camera_pos, camera_up, proj_mat)
        .Run();

    glfwTerminate();
    return 0;
}
