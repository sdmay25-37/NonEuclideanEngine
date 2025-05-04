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

#include <queue>

// TODO INCREASE RENDER DIST TO 4 BUT I USE 3 FOR LESSS LAG
#define rendDist 8

static float Theta = 27.0 * M_PI / 80.0f;
// static float Theta = 1.3;
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
    static void UpdateTile2(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap, Resource<Renderer> renderer, Resource<Window> window, std::string characterSprite)
    {
        // THIS FEELS UNCESSARY BUT IT MADE IT WORK
        // ASK BEN IF THIS IS GOOD ENOUGH
        // THIS ISNT ENOUGH ASK BEN
        auto view = registry.view<AtlasPQtile>();
        for (auto entity : view)
        {
            registry.destroy(entity);
        }
        auto view2 = registry.view<Character>();
        for (auto entity : view2)
        {
            registry.destroy(entity);
        }
        auto view3 = registry.view<Rock>();
        for (auto entity : view3)
        {
            registry.destroy(entity);
        }

        // TODO INCREASE RENDER DIST TO 4 BUT I USE 3 FOR LESSS LAG
        std::vector<Tile> nearTiles = tilemap->getNearTiles(tilemap->currentTile, rendDist + 3);
        PQTile SpriteTile = PQTile(4, 5, COLOR::WHITE);
        Tile root_tile = tilemap->currentTile;
        SpriteTile.to_weirstrass();

        std::unordered_map<int, int> *processed_tiles = new std::unordered_map<int, int>();
        addTileAndNeighborsBFS2(registry, texture_manager, root_tile, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);

        // std::unordered_set<int> processed_tiles;
        // addTileAndNeighbors(registry, texture_manager, root_tile, 0, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);

        // Character Tile
        PQTile SpriteTile2 = PQTile(4, 5, COLOR::WHITE);
        SpriteTile2.scale(0.5f);
        SpriteTile2.to_weirstrass();
        SpriteTile2.recalculate_uvs();
        const auto entity = registry.create();

        // todo update sprite
        // std::cout << characterSprite << "\n";
        auto texture_result = texture_manager->getTexture(characterSprite);
        if (texture_result)
        {
            AtlasedTexture texture = texture_result.value();
            registry.emplace<Character>(entity, SpriteTile2, texture);
        }
        else
        {
            std::cout << "Error: Failed to load texture '" << characterSprite << "'!" << "\n";
        }
    }

    static void CreateTiles3(entt::registry &registry, Resource<TextureManager> texture_manager, Resource<TileMap> tilemap)
    {
        Tile currentTile = tilemap->getTileByID(25);
        std::cout << glGetString(GL_VERSION);
        std::vector<Tile> nearTiles2 = tilemap->getNearTiles(currentTile, rendDist + 3);
        PQTile SpriteTile = PQTile(4, 5, COLOR::WHITE);
        Tile root_tile = currentTile;
        SpriteTile.to_weirstrass();

        std::unordered_map<int, int> *processed_tiles = new std::unordered_map<int, int>();
        addTileAndNeighborsBFS2(registry, texture_manager, root_tile, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);

        // std::unordered_set<int> processed_tiles;
        // addTileAndNeighbors(registry, texture_manager, root_tile, 0, rendDist, root_tile.sprite, SpriteTile, tilemap, processed_tiles);

        // Character Tile
        PQTile SpriteTile2 = PQTile(4, 5, COLOR::WHITE);
        SpriteTile2.scale(0.5f);
        SpriteTile2.to_weirstrass();
        SpriteTile2.recalculate_uvs();
        const auto entity = registry.create();
        auto texture_result = texture_manager->getTexture("character.png");
        if (texture_result)
        {
            AtlasedTexture texture = texture_result.value();
            registry.emplace<Character>(entity, SpriteTile2, texture);
        }
        else
        {
            std::cout << "Error: Failed to load texture '" << "character.png" << "'!" << "\n";
        }
    }

    static void addTileAndNeighborsBFS2(entt::registry &registry, Resource<TextureManager> texture_manager,
                                        const Tile &root_tile, int radius,
                                        const std::string &texturePath, const PQTile &Sprite_tile,
                                        Resource<TileMap> tilemap,
                                        std::unordered_map<int, int> *processed_tiles)
    {
        std::queue<std::tuple<Tile, int, PQTile>> tile_queue;

        // Immediately register root
        {
            auto entity = registry.create();
            auto texture_result = texture_manager->getTexture(root_tile.sprite);
            if (texture_result)
            {
                AtlasedTexture texture = texture_result.value();
                PQTile root_tile_copy = Sprite_tile; // Make a modifiable copy
                root_tile_copy.recalculate_uvs();

                if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
                {
                    auto texture_result2 = texture_manager->getTexture("grass1.png");
                    if (texture_result2)
                    {
                        AtlasedTexture texture2 = texture_result2.value();
                        const auto entity2 = registry.create();
                        registry.emplace<AtlasPQtile>(entity2, root_tile_copy, texture2, (float)0.0);
                    }
                    root_tile_copy.recalculate_uvs2();

                    registry.emplace<Rock>(entity, root_tile_copy, texture);
                }
                else
                {
                    registry.emplace<AtlasPQtile>(entity, root_tile_copy, texture, (float)0.0);
                }
                // registry.emplace<AtlasPQtile>(entity, root_tile_copy, texture, 0.0f);
                // std::cout << "Added root tile ID: " << root_tile._tileId << "\n";
            }
        }

        processed_tiles->insert({root_tile._tileId, 0});
        tile_queue.push({root_tile, 0, Sprite_tile});

        while (!tile_queue.empty())
        {
            auto [tile, currentRelation, pq_tile] = tile_queue.front();
            tile_queue.pop();

            if (currentRelation >= radius || tile._tileId == -1)
                continue;

            // Left neighbor
            if (tile._leftTileId != -1)
            {
                Tile left_tile = tilemap->getTileInRenderedList(tile._leftTileId);
                if (left_tile._tileId != -1)
                {
                    // Rotate left neighbor around Y-axis by -Theta
                    PQTile rotated_left_tile = pq_tile;
                    rotated_left_tile.rotateYHyperbolic(-Theta);

                    // Check if we've seen this tile ID at a lower or same relation
                    auto it = processed_tiles->find(left_tile._tileId);
                    if (it == processed_tiles->end() || it->second == currentRelation)
                    {
                        // Add to registry
                        auto entity = registry.create();
                        auto texture_result = texture_manager->getTexture(left_tile.sprite);

                        if (texture_result)
                        {
                            AtlasedTexture texture = texture_result.value();
                            rotated_left_tile.recalculate_uvs();

                            if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
                            {
                                auto texture_result2 = texture_manager->getTexture("grass1.png");
                                if (texture_result2)
                                {
                                    AtlasedTexture texture2 = texture_result2.value();
                                    const auto entity2 = registry.create();
                                    registry.emplace<AtlasPQtile>(entity2, rotated_left_tile, texture2, (float)0.0);
                                }

                                rotated_left_tile.recalculate_uvs2();

                                registry.emplace<Rock>(entity, rotated_left_tile, texture);
                            }
                            else
                            {
                                registry.emplace<AtlasPQtile>(entity, rotated_left_tile, texture, (float)0.0);
                            }

                            // registry.emplace<AtlasPQtile>(entity, rotated_left_tile, texture, 0.0f);
                            // std::cout << "Added left tile ID: " << left_tile._tileId << " at relation " << currentRelation << "\n";
                        }

                        // Track minimum seen relation
                        (*processed_tiles)[left_tile._tileId] = currentRelation;
                        tile_queue.push({left_tile, currentRelation + 1, rotated_left_tile});
                    }
                }
            }

            // Right neighbor
            if (tile._rightTileId != -1)
            {
                Tile right_tile = tilemap->getTileInRenderedList(tile._rightTileId);
                if (right_tile._tileId != -1)
                {
                    // Rotate right neighbor around Y-axis by Theta
                    PQTile rotated_right_tile = pq_tile;
                    rotated_right_tile.rotateYHyperbolic(Theta);

                    // Check if we've seen this tile ID at a lower or same relation
                    auto it = processed_tiles->find(right_tile._tileId);
                    if (it == processed_tiles->end() || it->second == currentRelation)
                    {
                        // Add to registry
                        auto entity = registry.create();
                        auto texture_result = texture_manager->getTexture(right_tile.sprite);
                        if (texture_result)
                        {
                            AtlasedTexture texture = texture_result.value();
                            rotated_right_tile.recalculate_uvs();

                            if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
                            {
                                auto texture_result2 = texture_manager->getTexture("grass1.png");
                                if (texture_result2)
                                {
                                    AtlasedTexture texture2 = texture_result2.value();
                                    const auto entity2 = registry.create();
                                    registry.emplace<AtlasPQtile>(entity2, rotated_right_tile, texture2, (float)0.0);
                                }
                                rotated_right_tile.recalculate_uvs2();

                                registry.emplace<Rock>(entity, rotated_right_tile, texture);
                            }
                            else
                            {
                                registry.emplace<AtlasPQtile>(entity, rotated_right_tile, texture, (float)0.0);
                            }
                            // registry.emplace<AtlasPQtile>(entity, rotated_right_tile, texture, 0.0f);
                            // std::cout << "Added right tile ID: " << right_tile._tileId << " at relation " << currentRelation << "\n";
                        }

                        // Track minimum seen relation
                        (*processed_tiles)[right_tile._tileId] = currentRelation;
                        tile_queue.push({right_tile, currentRelation + 1, rotated_right_tile});
                    }
                }
            }

            // Up neighbor
            if (tile._upTileId != -1)
            {
                Tile up_tile = tilemap->getTileInRenderedList(tile._upTileId);
                if (up_tile._tileId != -1)
                {
                    // Rotate up neighbor around X-axis by Theta
                    PQTile rotated_up_tile = pq_tile;
                    rotated_up_tile.rotateXHyperbolic(Theta);

                    // Check if we've seen this tile ID at a lower or same relation
                    auto it = processed_tiles->find(up_tile._tileId);
                    if (it == processed_tiles->end() || it->second == currentRelation)
                    {
                        // Add to registry
                        auto entity = registry.create();
                        auto texture_result = texture_manager->getTexture(up_tile.sprite);
                        if (texture_result)
                        {
                            AtlasedTexture texture = texture_result.value();
                            rotated_up_tile.recalculate_uvs();

                            if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
                            {
                                auto texture_result2 = texture_manager->getTexture("grass1.png");
                                if (texture_result2)
                                {
                                    AtlasedTexture texture2 = texture_result2.value();
                                    const auto entity2 = registry.create();
                                    registry.emplace<AtlasPQtile>(entity2, rotated_up_tile, texture2, (float)0.0);
                                }
                                rotated_up_tile.recalculate_uvs2();

                                registry.emplace<Rock>(entity, rotated_up_tile, texture);
                            }
                            else
                            {
                                registry.emplace<AtlasPQtile>(entity, rotated_up_tile, texture, (float)0.0);
                            }
                            // registry.emplace<AtlasPQtile>(entity, rotated_up_tile, texture, 0.0f);
                            // std::cout << "Added up tile ID: " << up_tile._tileId << " at relation " << currentRelation << "\n";
                        }

                        // Track minimum seen relation
                        (*processed_tiles)[up_tile._tileId] = currentRelation;
                        tile_queue.push({up_tile, currentRelation + 1, rotated_up_tile});
                    }
                }
            }

            // Down neighbor
            if (tile._downTileId != -1)
            {
                Tile down_tile = tilemap->getTileInRenderedList(tile._downTileId);
                if (down_tile._tileId != -1)
                {
                    // Rotate down neighbor around X-axis by -Theta
                    PQTile rotated_down_tile = pq_tile;
                    rotated_down_tile.rotateXHyperbolic(-Theta);

                    // Check if we've seen this tile ID at a lower or same relation
                    auto it = processed_tiles->find(down_tile._tileId);
                    if (it == processed_tiles->end() || it->second == currentRelation)
                    {
                        // Add to registry
                        auto entity = registry.create();
                        auto texture_result = texture_manager->getTexture(down_tile.sprite);
                        if (texture_result)
                        {
                            AtlasedTexture texture = texture_result.value();
                            rotated_down_tile.recalculate_uvs();

                            if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
                            {
                                auto texture_result2 = texture_manager->getTexture("grass1.png");
                                if (texture_result2)
                                {
                                    AtlasedTexture texture2 = texture_result2.value();
                                    const auto entity2 = registry.create();
                                    registry.emplace<AtlasPQtile>(entity2, rotated_down_tile, texture2, (float)0.0);
                                }
                                rotated_down_tile.recalculate_uvs2();

                                registry.emplace<Rock>(entity, rotated_down_tile, texture);
                            }
                            else
                            {
                                registry.emplace<AtlasPQtile>(entity, rotated_down_tile, texture, (float)0.0);
                            }
                            // registry.emplace<AtlasPQtile>(entity, rotated_down_tile, texture, 0.0f);
                            // std::cout << "Added down tile ID: " << down_tile._tileId << " at relation " << currentRelation << "\n";
                        }

                        // Track minimum seen relation
                        (*processed_tiles)[down_tile._tileId] = currentRelation;
                        tile_queue.push({down_tile, currentRelation + 1, rotated_down_tile});
                    }
                }
            }
        }
    }

    static void
    addTileAndNeighbors(entt::registry &registry, Resource<TextureManager> texture_manager,
                        const Tile &root_tile, int currentRelation, int radius,
                        const std::string &texturePath, const PQTile &Sprite_tile, Resource<TileMap> tilemap, std::unordered_set<int> processed_tiles)
    {

        if (currentRelation >= radius)
        {
            return;
        }
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
        auto texture_result = texture_manager->getTexture(texturePath);
        if (texture_result)
        {
            AtlasedTexture texture = texture_result.value();
            PQTile render_Tile = Sprite_tile;
            render_Tile.recalculate_uvs();
            if (texturePath != "grass1.png" | texturePath != "grass2.png" | texturePath != "grass3.png" | texturePath != "grass4.png")
            {
                auto texture_result2 = texture_manager->getTexture("grass1.png");
                if (texture_result2)
                {
                    AtlasedTexture texture2 = texture_result2.value();
                    const auto entity2 = registry.create();
                    registry.emplace<AtlasPQtile>(entity2, render_Tile, texture2, (float)0.0);
                }

                registry.emplace<Rock>(entity, render_Tile, texture);
            }
            else
            {
                registry.emplace<AtlasPQtile>(entity, render_Tile, texture, (float)0.0);
            }
        }
        else
        {
            std::cout << "Error: Failed to load texture '" << texturePath << "'!" << "\n";
            return;
        }

        // Create neighbor tiles and rotate them accordingly
        // Clone the original tile to prevent modifying the input
        if (root_tile._leftTileId != -1 || tilemap->getTileInRenderedList(root_tile._leftTileId)._tileId != -1)
        {
            // std::cout << "LEFT TILE: " << tilemap->getTileInRenderedList(root_tile._leftTileId).to_string() << std::endl;
            // std::cout << "From TILE: " << root_tile.to_string() << std::endl;
            // std::cout << "Current Dist: " << currentRelation << std::endl;

            Tile tile_left = tilemap->getTileInRenderedList(root_tile._leftTileId);
            PQTile left_tile = Sprite_tile;
            left_tile.rotateYHyperbolic(-Theta);
            addTileAndNeighbors(registry, texture_manager, tile_left, currentRelation + 1, radius, tile_left.sprite, left_tile, tilemap, processed_tiles);
        }
        if (root_tile._rightTileId != -1 || tilemap->getTileInRenderedList(root_tile._downTileId)._rightTileId != -1)
        {

            Tile tile_right = tilemap->getTileInRenderedList(root_tile._rightTileId);
            PQTile right_tile = Sprite_tile;
            right_tile.rotateYHyperbolic(Theta);
            addTileAndNeighbors(registry, texture_manager, tile_right, currentRelation + 1, radius, tile_right.sprite, right_tile, tilemap, processed_tiles);
        }
        if (root_tile._upTileId != -1 || tilemap->getTileInRenderedList(root_tile._upTileId)._tileId != -1)
        {
            Tile tile_top = tilemap->getTileInRenderedList(root_tile._upTileId);
            PQTile top_tile = Sprite_tile;
            top_tile.rotateXHyperbolic(Theta);
            addTileAndNeighbors(registry, texture_manager, tile_top, currentRelation + 1, radius, tile_top.sprite, top_tile, tilemap, processed_tiles);
        }
        if (root_tile._downTileId != -1 || tilemap->getTileInRenderedList(root_tile._downTileId)._tileId != -1)
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
        tileMap->loadTiles("../tests/json/forest_test_small3.json");
    }

    static void MoveCamera(Resource<Camera> camera, Resource<Input> input, Resource<TileMap> tilemap, entt::registry &registry, Resource<TextureManager> texture_manager, Resource<Renderer> renderer, Resource<Window> window)
    {
        const float moveCooldown = 0.01f; // ADDED DELAY SO THAT MOVEMENT IS SLOWER AND IS FLUID

        float deltaTime = ImGui::GetIO().DeltaTime; // USING IMGUI because it was already in here
        timeSinceLastMove += deltaTime;
        if (timeSinceLastMove < moveCooldown)
            return;

        else if (input->isKeyPressed(GLFW_KEY_W))
        {
            if (tilemap->currentTile._upTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._upTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._upTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer, window, "character_up.PNG");
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_A))
        {
            if (tilemap->currentTile._leftTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._leftTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer, window, "character_left.PNG");
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_S))
        {
            if (tilemap->currentTile._downTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._downTileId), tilemap))
            {
                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._downTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer, window, "character_down.PNG");
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->isKeyPressed(GLFW_KEY_D))
        {
            if (tilemap->currentTile._rightTileId != -1 && isValidTileToMove(tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId), tilemap))
            {

                tilemap->currentTile = tilemap->getTileInRenderedList(tilemap->currentTile._rightTileId);
                UpdateTile2(registry, texture_manager, tilemap, renderer, window, "character_right.PNG");
                timeSinceLastMove = 0.0f;
            }
        }
        else if (input->wasKeyPressed(GLFW_KEY_R))
        {
            std::cout << tilemap->currentTile.to_string() << "\n";

            std::unordered_map<std::string, entt::entity> debug_tile_positions; // key = "x_y", value = entity

            // Compute center point from poly_vertices
            auto view = registry.view<AtlasPQtile>();
            for (auto entity : view)
            {
                AtlasPQtile &atlas_tile = view.get<AtlasPQtile>(entity);
                PQTile &pq_tile = atlas_tile.tile;
                glm::vec2 center(0.0f, 0.0f);

                for (const auto &pt : pq_tile.poly_mesh)
                {
                    center.x += pt.x;
                    center.y += pt.y;
                }
                center.x /= static_cast<float>(pq_tile.poly_mesh.size());
                center.y /= static_cast<float>(pq_tile.poly_mesh.size());

                // Create a simple string key from center
                std::string position_key = std::to_string(center.x) + "_" + std::to_string(center.y);

                // Debug: check for overlap
                constexpr float tolerance = 0.001f;
                bool found_overlap = false;
                for (const auto &[existing_key, existing_entity] : debug_tile_positions)
                {
                    float ex, ey;
                    sscanf(existing_key.c_str(), "%f_%f", &ex, &ey);

                    if (std::abs(center.x - ex) < tolerance && std::abs(center.y - ey) < tolerance)
                    {
                        std::cout << "Debug: Overlapping tiles! "
                                  << "Entity " << (int)entity << " and Entity " << (int)existing_entity
                                  << " are at (" << center.x << ", " << center.y << ")\n";
                        found_overlap = true;
                        break;
                    }
                }

                if (!found_overlap)
                {
                    debug_tile_positions[position_key] = entity;
                }
                renderer->Clear();
                renderer->Render(registry, camera);
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
        if (tile.sprite == "rock1.png" | tile.sprite == "rock2.png" | tile.sprite == "stump.png")
        {
            return false;
        }

        return true;
    }
};

int main()
{

    // glm::vec3 camera_pos(0.0, 0.0, 2.0);
    // glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

    // float fov = glm::radians(45.0f);
    // float nearPlane = 0.1f;
    // float farPlane = 100.0f;

    // glm::mat4 proj_mat = glm::perspective(fov, (1080.0f / 1080.0f), nearPlane, farPlane);

    glm::vec3 camera_pos(0.0f, 0.0f, 2.0f);
    glm::vec4 camera_up(0.0f, 1.0f, 0.0f, 1.0);

    float nearPlane = -1.0f;
    float farPlane = 100.0f;

    float orthoLeft = -1.0f;
    float orthoRight = 1.0f;
    float orthoBottom = -1.0f;
    float orthoTop = 1.0f;

    float windowWidth = 1080.0f;
    float windowHeight = 1080.0f;
    // Scale the ortho bounds if your window isn't square
    if (windowWidth > windowHeight)
    {
        float ratio = (float)windowWidth / (float)windowHeight;
        orthoLeft *= ratio;
        orthoRight *= ratio;
    }
    else
    {
        float ratio = (float)windowHeight / (float)windowWidth;
        orthoBottom *= ratio;
        orthoTop *= ratio;
    }

    glm::mat4 proj_mat = glm::ortho(
        orthoLeft,
        orthoRight,
        orthoBottom,
        orthoTop,
        nearPlane,
        farPlane);

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
