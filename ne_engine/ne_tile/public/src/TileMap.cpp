//
//  Authors: Tasman Grinnell, Cory Roth
//  2/2/2025
//

#include "TileMap.hpp"

#include <fstream>
#include <iostream>
#include <queue>

using json = nlohmann::json;

TileMap::TileMap(std::unordered_map<int, Tile> tileList, std::vector<Tile> seedList)
{
    _tileList = tileList;
    _seedList = seedList;
}

// // Loads tiles from JSON
// void TileMap::loadTiles(const std::string &filename)
// {
// std::ifstream inputFile(filename);
// nlohmann::json tileData = nlohmann::json::parse(inputFile);

// if (tileData.size() == 0)
// {
//     // JSON File is empty
//     std::cerr << "JSON File empty" << std::endl;
//     throw std::length_error("JSON File empty");
// }

// for (const auto &item : tileData)
//     {
//         Tile tile;
//         tile._tileId = item.at("tileId").get<uint8_t>();
//         tile._spriteId = item.at("spriteId").get<uint8_t>();
//         tile._tileType = item.at("tileType").get<std::string>();
//         tile._properties = item.at("properties").get<std::vector<std::string>>();
//         tile._upTileId = item.at("upTileId").get<uint8_t>();
//         tile._downTileId = item.at("downTileId").get<uint8_t>();
//         tile._leftTileId = item.at("leftTileId").get<uint8_t>();
//         tile._rightTileId = item.at("rightTileId").get<uint8_t>();

//         _tileList[tile._tileId] = tile;
//     }
// }

void TileMap::loadTiles(const std::string &filename)
{
    std::ifstream inputFile(filename);
    nlohmann::json tileData = nlohmann::json::parse(inputFile);

    if (tileData.size() == 0)
    {
        // JSON File is empty
        std::cerr << "JSON File empty" << std::endl;
        throw std::length_error("JSON File empty");
    }

    numTiles = tileData.size();
    for (const auto &item : tileData)
    {

        Tile tile;

        // Ensure each key exists before accessing it
        if (item.contains("tileId"))
            tile._tileId = item["tileId"].get<int>();
        else
            tile._tileId = -1; // Default invalid ID

        if (item.contains("spriteId"))
            tile._spriteId = item["spriteId"].get<int8_t>();
        else
            tile._spriteId = -1; // Default sprite ID

        if (item.contains("sprite"))
            tile.sprite = item["sprite"].get<std::string>();
        else
            tile.sprite = "cy.jpg"; // Default sprite ID

        if (item.contains("tileType"))
            tile._tileType = item["tileType"].get<std::string>();
        else
            tile._tileType = "unknown"; // Default tile type

        if (item.contains("properties") && item["properties"].is_array())
            tile._properties = item["properties"].get<std::vector<std::string>>();
        else
            tile._properties = {}; // Empty properties if missing

        if (item.contains("upTileId"))
            tile._upTileId = item["upTileId"].get<int>();
        else
            tile._upTileId = -1;

        if (item.contains("downTileId"))
            tile._downTileId = item["downTileId"].get<int>();
        else
            tile._downTileId = -1;

        if (item.contains("leftTileId"))
            tile._leftTileId = item["leftTileId"].get<int>();
        else
            tile._leftTileId = -1;

        if (item.contains("rightTileId"))
            tile._rightTileId = item["rightTileId"].get<int>();
        else
            tile._rightTileId = -1;

        // Parse world position if present
        if (item.contains("worldPosition") && item["worldPosition"].is_array() && item["worldPosition"].size() == 2)
        {
            tile.worldPosition.first = item["worldPosition"][0].get<int>();
            tile.worldPosition.second = item["worldPosition"][1].get<int>();
        }
        else
        {
            tile.worldPosition = {-1, -1}; // TODO change but for testing if it does not exist
        }

        // Parse relationToCurrentTile
        if (item.contains("relationToCurrentTile"))
            tile.relationToCurrentTile = item["relationToCurrentTile"].get<int8_t>();
        else
            tile.relationToCurrentTile = -1; // Default relation value

        _tileList[tile._tileId] = tile;
    }

    currentTile = getTileByID(85);
    std::cout << "\n"
              << "Tiles Loaded" << "\n";
}

// Used for testing
std::vector<std::string> TileMap::getTileMapInformation()
{
    std::vector<std::string> returnTiles;
    returnTiles.resize(_tileList.size());
    int i = 0;
    for (const auto &pair : _tileList)
    {
        const Tile &tile = pair.second;
        returnTiles.at(i) = tile.to_string();
        i++;
    }

    return returnTiles;
}

// Does bfs for near tiles
std::vector<Tile> TileMap::getNearTiles(Tile currentTile, int radius)
{
    std::vector<Tile> nearTiles;
    std::unordered_map<int, Tile> visitedTiles;

    // Start with the current tile
    visitedTiles[currentTile._tileId] = currentTile;
    currentTile.relationToCurrentTile = 0;
    currentTile.relationMappingToCurrentTile = {0, 0};
    nearTiles.push_back(currentTile);
    this->currentTile = currentTile;

    std::queue<std::pair<Tile, int>> queue; // Queue holds tile and depth
    queue.push({currentTile, 0});           // Start BFS with currentTile at depth 0

    while (!queue.empty())
    {
        std::pair<Tile, int> front = queue.front();
        queue.pop();

        Tile tile = front.first;
        int depth = front.second;

        // Stop if we've reached the max depth (radius)
        if (depth >= radius)
            continue;

        // Explore neighboring tiles (up, down, left, right)
        if (tile._upTileId != -1 && visitedTiles.find(tile._upTileId) == visitedTiles.end())
        {
            Tile upTile = getTileByID(tile._upTileId);
            visitedTiles[upTile._tileId] = upTile;
            upTile.relationToCurrentTile = depth + 1;
            upTile.relationMappingToCurrentTile = {upTile.worldPosition.first - currentTile.worldPosition.first, upTile.worldPosition.second - currentTile.worldPosition.second};
            nearTiles.push_back(upTile);
            queue.push({upTile, depth + 1});
        }

        if (tile._downTileId != -1 && visitedTiles.find(tile._downTileId) == visitedTiles.end())
        {
            Tile downTile = getTileByID(tile._downTileId);
            visitedTiles[downTile._tileId] = downTile;
            downTile.relationToCurrentTile = depth + 1;
            downTile.relationMappingToCurrentTile = {downTile.worldPosition.first - currentTile.worldPosition.first, downTile.worldPosition.second - currentTile.worldPosition.second};
            nearTiles.push_back(downTile);
            queue.push({downTile, depth + 1});
        }

        if (tile._leftTileId != -1 && visitedTiles.find(tile._leftTileId) == visitedTiles.end())
        {
            Tile leftTile = getTileByID(tile._leftTileId);
            visitedTiles[leftTile._tileId] = leftTile;
            leftTile.relationToCurrentTile = depth + 1;
            leftTile.relationMappingToCurrentTile = {leftTile.worldPosition.first - currentTile.worldPosition.first, leftTile.worldPosition.second - currentTile.worldPosition.second};
            nearTiles.push_back(leftTile);
            queue.push({leftTile, depth + 1});
        }

        if (tile._rightTileId != -1 && visitedTiles.find(tile._rightTileId) == visitedTiles.end())
        {
            Tile rightTile = getTileByID(tile._rightTileId);
            visitedTiles[rightTile._tileId] = rightTile;
            rightTile.relationToCurrentTile = depth + 1;
            rightTile.relationMappingToCurrentTile = {rightTile.worldPosition.first - currentTile.worldPosition.first, rightTile.worldPosition.second - currentTile.worldPosition.second};
            nearTiles.push_back(rightTile);
            queue.push({rightTile, depth + 1});
        }
    }

    _renderedTileList = nearTiles;
    return nearTiles;
}

Tile TileMap::getTileByID(int tileId)
{
    // Check if the tileId exists in the tileList
    auto it = _tileList.find(tileId);

    // If found, return the tile
    if (it != _tileList.end())
    {
        return it->second; // Return the Tile object associated with the tileId
    }

    std::cerr << "Error: Tile with ID " << static_cast<int>(tileId) << " not found!" << std::endl;

    // TODO FIGURE OUT WHAT TO SEND BACK
    return Tile(); // Assuming the default constructor of Tile initializes a valid empty object
}

Tile TileMap::getTileInRenderedList(int tileId)
{
    auto it = std::find_if(_renderedTileList.begin(), _renderedTileList.end(),
                           [&](const Tile &tile)
                           { return tile._tileId == tileId; });

    if (it != _renderedTileList.end())
    {
        // std::cout << "Tile found!\n";
        return *it; // Return the found tile
    }
    else
    {
        std::cout << "Tile not found!\n";
        return Tile(); // TODO Update Return a default Tile if not found
    }
}
