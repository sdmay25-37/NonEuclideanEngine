#define GLM_FORCE_CTOR_INIT

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

#include "ne_engine.hpp"

#include "TileMap.hpp"
#include "Tile.hpp"

int main()
{
    TileMap *tilemap = new TileMap();

    tilemap->loadTiles("json/testTileMap.json");

    std::vector<std::string> info = tilemap->getTileMapInformation();

    for (int i = 0; i < info.size(); i++)
    {
        std::cout << info.at(i);
    }
    std::cout << "\n\n\n\n\n";
    TileMap tileMap2;
    tileMap2.loadTiles("json/testNear.json"); // Ensure the path to your JSON file is correct

    // Choose a starting tile to test
    Tile startTile = tileMap2.getTileByID(5); // Assuming we start with tile ID 1

    std::cout << '\n'
              << "Tile 5:" << startTile.to_string() << "\n";
    // Test the getNearTiles function with a radius of 1
    int radius = 3;
    std::vector<Tile> nearTiles = tileMap2.getNearTiles(startTile, radius);

    // Print the results
    std::cout << "Near tiles for Tile ID " << static_cast<int>(startTile._tileId) << " with radius " << radius << ":\n";
    for (const Tile &tile : nearTiles)
    {
        std::cout << tile.to_string() << "\n";
    }

    return 0;

    return 0;
}
