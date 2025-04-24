#include <stb/stb_image.h>

#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include "ne_engine.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <math.h>

#include "TileMap.hpp"
#include "Tile.hpp"
#include "Input.hpp"

// TODO INCREASE RENDER DIST TO 4 BUT I USE 3 FOR LESSS LAG
#define rendDist 6

static float Theta = M_PI / 3.0f;
// BRO TRUST THIS IS IMPORTANT
static float timeSinceLastMove = 0.0f;

class WorldPlugin final : public Plugin
{
public:
    void Build(App &app) override
    {
        app
            .AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(CreateTiles3).After(LoadTextures).After(LoadTiles).After(BindInput)))
            .AddSystems(ScheduleLabel::UPDATE, std::move(SystemSet(MoveCamera)));
    }

private:
    static void UpdateTile2(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap, Resource<Renderer> renderer)
    {
        // THIS FEELS UNCESSARY BUT IT MADE IT WORK
        // ASK BEN IF THIS IS GOOD ENOUGH
        // THIS ISNT ENOUGH ASK BEN
        auto view = registry.view<AtlasPQtile>();
        for (auto entity : view)
        {
            registry.destroy(entity);
        }
        auto view2 = registry.view<AtlasPQtile>();
        // TODO INCREASE RENDER DIST TO 4 BUT I USE 3 FOR LESSS LAG
        std::vector<Tile> nearTiles = tilemap->getNearTiles(tilemap->currentTile, rendDist);
        PQTile SpriteTile = PQTile(4, 5, COLOR::WHITE);
        Tile root_tile = tilemap->currentTile;
        SpriteTile.to_weirstrass();
        std::unordered_set<int> processed_tiles;
        addTileAndNeighbors(registry, texture_manager, root_tile, 0, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);
    }

    static void CreateTiles3(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        Tile currentTile = tilemap->getTileByID(82);

        std::vector<Tile> nearTiles2 = tilemap->getNearTiles(currentTile, rendDist);
        PQTile SpriteTile = PQTile(4, 5, COLOR::WHITE);
        Tile root_tile = currentTile;
        SpriteTile.to_weirstrass();
        std::unordered_set<int> processed_tiles;
        addTileAndNeighbors(registry, texture_manager, root_tile, 0, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);
    }

    static void addTileAndNeighbors(entt::registry &registry, Resource<TextureManager> texture_manager,
                                    const Tile &root_tile, int currentRelation, int radius,
                                    const std::string &texturePath, const PQTile &Sprite_tile, Resource<TileMap> tilemap, std::unordered_set<int> processed_tiles)
    {
        if (currentRelation >= radius)
            return;

        if (processed_tiles.find(root_tile._tileId) != processed_tiles.end())
        {
            return; // Tile has already been added, exit the function
        }
        if (root_tile._tileId == -1)
        {
            return;
        }

        // Add the tile to the processed set
        processed_tiles.insert(root_tile._tileId);
        // Create entity and add tile to registry
        const auto entity = registry.create();
        // std::cout << "HERE:'" << texturePath << "'!" << "\n";
        auto texture_result = texture_manager->getTexture(texturePath);
        if (texture_result)
        {
            AtlasedTexture texture = texture_result.value();
            // std::cout << "Tile Added: " << root_tile._tileId << "\n";
            // for (int ID : processed_tiles)
            // {
            //     std::cout << "Processed_Tiles" << ID << "\n";
            // }
            registry.emplace<AtlasPQtile>(entity, Sprite_tile, texture);
        }
        else
        {
            std::cout << "Error: Failed to load texture '" << texturePath << "'!" << "\n";
            return;
        }

        // Create neighbor tiles and rotate them accordingly
        // Clone the original tile to prevent modifying the input
        if (root_tile._leftTileId != -1 && tilemap->getTileInRenderedList(root_tile._leftTileId)._tileId != -1)
        {
            Tile tile_left = tilemap->getTileInRenderedList(root_tile._leftTileId);
            PQTile left_tile = Sprite_tile;
            left_tile.rotateYHyperbolic(-Theta);
            addTileAndNeighbors(registry, texture_manager, tile_left, currentRelation + 1, radius, tile_left.sprite, left_tile, tilemap, processed_tiles);
        }
        if (root_tile._rightTileId != -1 && tilemap->getTileInRenderedList(root_tile._downTileId)._rightTileId != -1)
        {

            Tile tile_right = tilemap->getTileInRenderedList(root_tile._rightTileId);
            PQTile right_tile = Sprite_tile;
            right_tile.rotateYHyperbolic(Theta);
            addTileAndNeighbors(registry, texture_manager, tile_right, currentRelation + 1, radius, tile_right.sprite, right_tile, tilemap, processed_tiles);
        }
        if (root_tile._upTileId != -1 && tilemap->getTileInRenderedList(root_tile._upTileId)._tileId != -1)
        {
            Tile tile_top = tilemap->getTileInRenderedList(root_tile._upTileId);
            PQTile top_tile = Sprite_tile;
            top_tile.rotateXHyperbolic(Theta);
            addTileAndNeighbors(registry, texture_manager, tile_top, currentRelation + 1, radius, tile_top.sprite, top_tile, tilemap, processed_tiles);
        }
        if (root_tile._downTileId != -1 && tilemap->getTileInRenderedList(root_tile._downTileId)._tileId != -1)
        {
            Tile tile_bottom = tilemap->getTileInRenderedList(root_tile._downTileId);
            PQTile bottom__tile = Sprite_tile;
            bottom__tile.rotateXHyperbolic(-Theta);
            addTileAndNeighbors(registry, texture_manager, tile_bottom, currentRelation + 1, radius, tile_bottom.sprite, bottom__tile, tilemap, processed_tiles);
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
        tileMap->loadTiles("../tests/json/maze_output_small.json");
    }

    static void MoveCamera(Resource<Camera> camera, Resource<Input> input, Resource<TileMap> tilemap, entt::registry &registry, Resource<TextureManager> texture_manager, Resource<Renderer> renderer)
    {
        const float moveCooldown = 0.0f; // ADDED DELAY SO THAT MOVEMENT IS SLOWER AND IS FLUID

        float deltaTime = ImGui::GetIO().DeltaTime; // USING IMGUI because it was already in here
        timeSinceLastMove += deltaTime;
        if (timeSinceLastMove < moveCooldown)
            return;

        else if (input->isKeyPressed(GLFW_KEY_W))
        {
            std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._upTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._upTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._upTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_A))
        {
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._leftTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_S))
        {
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._downTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._downTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._downTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer);
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_D))
        {
            // std::cout << tilemap->currentTile.to_string() << "\n";
            if (tilemap->currentTile._rightTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId), tilemap))
            {

                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer);
                timeSinceLastMove = 0.0f;
            }
        }
    }

    static void BindInput(Resource<Input> input, Resource<Window> window)
    {
        auto glfw_window = static_cast<GLFWwindow *>(window->get()); // Using get() to access GLFWwindow*
        input->BindWindow(glfw_window);
    }

    // Used to prevent moving onto a wall
    static bool isValidTileToMove(Tile tile, Resource<TileMap> tilemap)
    {
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
