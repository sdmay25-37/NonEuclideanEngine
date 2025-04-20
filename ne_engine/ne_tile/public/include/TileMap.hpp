//
//  Authors: Tasman Grinnell, Cory Roth
//  2/25/2025
//

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "Tile.hpp"

class TileMap
{
public:
    TileMap() = default;
    TileMap(std::unordered_map<int, Tile> tileList, std::vector<Tile> seedList);

    // load tiles from json
    void loadTiles(const std::string &filename);

    // NOT USED
    void saveTile(std::string filepath);

    // return a bunch of tiles from current tile
    std::vector<Tile> getNearTiles(Tile currentTile, int radius);

    // Search Every Loaded tile to find a tile
    // If not found returns a Tile with ID -1

    Tile getTileByID(int tileId);

    // Searches only the Tiles from last getNearTiles() call
    // If not found returns a Tile with ID -1
    Tile getTileInRenderedList(int tileID);

    // Number of tiles in _tileList
    int numTiles;

    // the tile last inputed from the last call of getNearTiles()
    Tile currentTile;

    // TODO: Figure out how to buffer tiles from _renderedTileList

    // Used for Testing
    std::vector<std::string>
    getTileMapInformation(); // Returns a vector of pairs of a <direction string, texture classification>

private:
    std::unordered_map<int, Tile> _tileList;

    // List of Tiles based on last getNearTiles() call
    std::vector<Tile> _renderedTileList;

    // Dont Know Purpose
    std::vector<Tile> _seedList; // Standard : [origin (0, 0), up , left , right , down]
    // ; If there are multiple entrances, then the ordering is to go clockwise from the leftmost upper Entrances
};

#endif // TILEMAP_HPP