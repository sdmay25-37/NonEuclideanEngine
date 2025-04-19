#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include "ne_engine.hpp"
#include "Polygon.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <math.h>

#include "TileMap.hpp"
#include "Tile.hpp"
#include "Input.hpp"

// BRO TRUST THIS IS IMPORTANT
static float timeSinceLastMove = 0.0f;

class WorldPlugin final : public Plugin
{
public:
    void Build(App &app) override
    {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles).After(LoadTextures).After(LoadTiles).After(BindInput)));
        // .AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    // used to render entire map
    /// NOT USED
    static void CreateTiles(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        std::srand(std::time(nullptr));

        // map is 162 * 162 (image is 162 by 162)
        int map_size = 4;
        float rect_size = 1.5 / map_size;
        float total_size = rect_size * map_size;

        Tile currentTile = tilemap->getTileByID(0);

        int num_sprites = map_size * map_size;

        PQTile tile = PQTile(4, 5, COLOR::WHITE);
        PQTile tile2 = PQTile(4, 5, COLOR::WHITE);
        PQTile tile3 = PQTile(4, 5, COLOR::WHITE);
        PQTile tile4 = PQTile(4, 5, COLOR::WHITE);

        // Convert tiles to Poincare representation
        std::vector<PQTile> tiles = {tile, tile2, tile3, tile4};

        std::cout
            << num_sprites << "\n";
        for (int i = 0; i < tilemap->numTiles; i++) // for (auto currentTile : tiles)
        {
            const auto entity = registry.create();

            PQTile currentTile = tiles.at(i % 4);
            std::cout << "Here" << "\n";

            currentTile.to_weirstrass(); // Ensures Poincaré conversion

            if (i == 1)
            {
                currentTile.rotateXHyperbolic(5.0 * M_PI / 16.0f);
            }
            else if (i == 2)
            {
                currentTile.rotateXHyperbolic(-5.0 * M_PI / 16.0f);
            }
            else if (i == 3)
            {
                currentTile.rotateYHyperbolic(5.0 * M_PI / 16.0f);
            }

            // Get texture from texture manager
            auto texture_result = texture_manager->getTexture("cy.jpg");

            // Check if texture was successfully retrieved
            if (texture_result)
            {
                AtlasedTexture texture = texture_result.value();
                // std::cout << "\n"
                //           << texture << "\n";
                // // Emplace the converted data into the registry
                // registry.emplace<AtlasMesh>(entity, positions, colors, uvs, indices_data, texture.atlas_id);
                registry.emplace<AtlasPQtile>(entity, currentTile, texture);

                std::cout << "Texture loaded successfully!" << "\n";
            }
            else
            {
                // Handle the error if the texture could not be retrieved
                std::cout << "Error: Failed to load texture 'cy.png'!" << "\n";
            }
        }
    }

    // Used to render tiles in a specific distance from a tile
    static void CreateTiles2(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        std::srand(std::time(nullptr));

        Tile currentTile = tilemap->getTileByID(85);
        // std::cout << tilemap->currentTile.to_string() << "\n";

        std::vector<Tile> nearTiles = tilemap->getNearTiles(currentTile, 20);
        // std::cout << tilemap->currentTile.to_string() << "\n";

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

    static void UpdateTile(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        // THIS FEELS UNCESSARY BUT IT MADE IT WORK
        // ASK BEN IF THIS IS GOOD ENOUGH
        auto view = registry.view<AtlasSprite>();
        for (auto entity : view)
        {
            registry.destroy(entity);
        }
        // std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAA" << "\n";

        std::vector<Tile> nearTiles = tilemap->getNearTiles(tilemap->currentTile, 20);
        int map_size = 20;
        float rect_size = 1.5 / map_size;
        float total_size = rect_size * map_size;

        int num_sprites = map_size * map_size;

        // std::cout << num_sprites << "\n";
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
        tileMap->loadTiles("../tests/json/testNear.json");
    }

    static void MoveCamera(Resource<Camera> camera, Resource<Input> input, Resource<TileMap> tilemap, entt::registry &registry, Resource<TextureManager> texture_manager)
    {
        const float moveCooldown = 1.0f; // ADDED DELAY SO THAT MOVEMENT IS SLOWER AND IS FLUID

        float deltaTime = ImGui::GetIO().DeltaTime; // USING IMGUI because it was already in here
        timeSinceLastMove += deltaTime;

        // std::cout << deltaTime << "\n";
        // std::cout << timeSinceLastMove << "\n";

        if (timeSinceLastMove < moveCooldown)
            return;

        else if (input->isKeyPressed(GLFW_KEY_W))
        {
            // camera->position.y += speed;
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._upTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._upTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._upTileId);
                UpdateTile(registry, texture_manager, tilemap);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_A))
        {
            // camera->position.x -= speed;
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._leftTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId);
                UpdateTile(registry, texture_manager, tilemap);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_S))
        {
            // camera->position.y -= speed;
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._downTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._downTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._downTileId);
                UpdateTile(registry, texture_manager, tilemap);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_D))
        {
            // camera->position.x += speed;
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._rightTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId);
                UpdateTile(registry, texture_manager, tilemap);
                timeSinceLastMove = 0.0f;
            }
        }
    }

    static void BindInput(Resource<Input> input, Resource<Window> window)
    {
        auto glfw_window = static_cast<GLFWwindow *>(window->get()); // Using get() to access GLFWwindow*
        input->BindWindow(glfw_window);
    }

    // Used to prevent moving from window
    static bool isValidTileToMove(Tile tile, Resource<TileMap> tilemap)
    {
        // std::cout << "TILEID \n"
        //           << tile._tileId << "\n";
        // std::cout << tile.to_string();
        // if (tile._tileId == -1)
        // {
        //     return false;
        // }
        if (tile.sprite == "wall.jpg")
        {
            return false;
        }

        return true;
    }
};

int main()
{

    glm::vec3 camera_pos(0.0, 0.0, 2.0);
    glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

    float fov = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::mat4 proj_mat = glm::perspective(fov, (1080.0f / 1080.0f), nearPlane, farPlane);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    App()
        .InsertResourceBase<Window, GLFWWindow>(1080, 1080)
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
