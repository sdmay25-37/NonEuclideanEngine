#include <iostream>

#include "App.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include "TileMap.hpp"
#include "Tile.hpp"
#include <stb/stb_image.h>

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
    tileMap2.loadTiles("json/testNear.json");

    // Choose a starting tile to test
    Tile startTile = tileMap2.getTileByID(5);

    std::cout << '\n'
              << "Tile 5:" << startTile.to_string() << "\n";
    // Test the getNearTiles function with a radius of 1
    int radius = 1;
    std::vector<Tile> nearTiles = tileMap2.getNearTiles(startTile, radius);

    std::cout << "Near tiles for Tile ID " << static_cast<int>(startTile._tileId) << " with radius " << radius << ":\n";
    for (const Tile &tile : nearTiles)
    {
        std::cout << tile.to_string() << "\n";
    }

    Tile test = tileMap2.getTileInRenderedList(2);
    Tile test2 = tileMap2.getTileInRenderedList(1);
    std::cout << test.to_string();
    std::cout << test2.to_string();
    return 0;
}
