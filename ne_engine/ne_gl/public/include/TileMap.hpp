//
//  Author: Tasman Grinnell
//  2/25/2025
//

/*
    Change to Tree structure
*/

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "Tile.hpp"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
class TileMap
{
public:
    TileMap();
    TileMap(std::unordered_map<int, Tile> tileList, std::vector<Tile> seedList);

    // load tiles from json
    void loadTiles(const std::string &filename);

    // NOT USED
    void saveTile(std::string filepath);

    // TODO return a bunch of tiles from cureent tile
    std::vector<Tile> getNearTiles(Tile currentTile, int radius);

    Tile getTileByID(uint8_t tileId);

    std::vector<std::string> getTileMapInformation(); // Returns a vector of pairs of a <direction string, texture classification>

private:
    std::unordered_map<int, Tile> _tileList;

    // Dont Know Purpose
    std::vector<Tile> _seedList; // Standard : [origin (0, 0), up , left , right , down]
    // ; If there are multiple entrances, then the ordering is to go clockwise from the leftmost upper Entrances
};

#endif // TILEMAP_HPP