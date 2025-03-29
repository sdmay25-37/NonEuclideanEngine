//
//  Authors: Tasman Grinnell, Cory Roth
//  2/2/2025
//

#include "TileMap.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <queue>

using json = nlohmann::json;

TileMap::TileMap()
{
}

TileMap::TileMap(std::unordered_map<int, Tile> tileList, std::vector<Tile> seedList)
{
    _tileList = tileList;
    _seedList = seedList;
}

// Loads tiles from JSON
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

    for (const auto &item : tileData)
    {
        Tile tile;
        tile._tileId = item.at("tileId").get<uint8_t>();
        tile._spriteId = item.at("spriteId").get<uint8_t>();
        tile._tileType = item.at("tileType").get<std::string>();
        tile._properties = item.at("properties").get<std::vector<std::string>>();
        tile._upTileId = item.at("upTileId").get<uint8_t>();
        tile._downTileId = item.at("downTileId").get<uint8_t>();
        tile._leftTileId = item.at("leftTileId").get<uint8_t>();
        tile._rightTileId = item.at("rightTileId").get<uint8_t>();

        _tileList[tile._tileId] = tile;
    }
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

std::vector<Tile> TileMap::getNearTiles(Tile currentTile, int radius)
{
    std::vector<Tile> nearTiles;
    std::unordered_map<int, Tile> visitedTiles;

    // Start with the current tile
    visitedTiles[currentTile._tileId] = currentTile;
    nearTiles.push_back(currentTile);

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
            nearTiles.push_back(upTile);
            queue.push({upTile, depth + 1});
        }

        if (tile._downTileId != -1 && visitedTiles.find(tile._downTileId) == visitedTiles.end())
        {
            Tile downTile = getTileByID(tile._downTileId);
            visitedTiles[downTile._tileId] = downTile;
            nearTiles.push_back(downTile);
            queue.push({downTile, depth + 1});
        }

        if (tile._leftTileId != -1 && visitedTiles.find(tile._leftTileId) == visitedTiles.end())
        {
            Tile leftTile = getTileByID(tile._leftTileId);
            visitedTiles[leftTile._tileId] = leftTile;
            nearTiles.push_back(leftTile);
            queue.push({leftTile, depth + 1});
        }

        if (tile._rightTileId != -1 && visitedTiles.find(tile._rightTileId) == visitedTiles.end())
        {
            Tile rightTile = getTileByID(tile._rightTileId);
            visitedTiles[rightTile._tileId] = rightTile;
            nearTiles.push_back(rightTile);
            queue.push({rightTile, depth + 1});
        }
    }

    return nearTiles;
}

Tile TileMap::getTileByID(uint8_t tileId)
{
    // Check if the tileId exists in the tileList
    auto it = _tileList.find(tileId);

    // If found, return the tile
    if (it != _tileList.end())
    {
        return it->second; // Return the Tile object associated with the tileId
    }
    else
    {
        std::cerr << "Error: Tile with ID " << static_cast<int>(tileId) << " not found!" << std::endl;

        // TODO FIGURE OUT WHAT TO SEND BACK
        return Tile(); // Assuming the default constructor of Tile initializes a valid empty object
    }
}
