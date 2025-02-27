//
//  Author: Tasman Grinnell
//  2/25/2025
//

/*
    Change to Tree structure
*/

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "Quadtree.h"

#include <vector>
#include <string>
#include <cstdint>

struct Tile {
    uint8_t tileId;
    Tile* up;
    Tile* down;
    Tile* left;
    Tile* right;
    char* tileType;     // Maybe change into an enum?
};

class TileMap {
public:
    TileMap();
    TileMap(std::vector<Tile> tileList, std::vector<Tile> seedList);

    void loadTiles(std::string filepath);  // Need to change this.  Trying to figure out a bit more about what the goal 
    // of the function is or if we can just pass a vector in for creating the list.

    std::vector <std::pair <char*, std::string>> getTileInformation();    // Returns a vector of pairs of a <direction string, texture classification>

private: 
    std::vector<Tile> _tileList;
    std::vector<Tile> _seedList;    // Standard : [origin (0, 0), up , left , right , down] 
    // ; If there are multiple entrances, then the ordering is to go clockwise from the leftmost upper Entrances
};

#endif // TILEMAP_HPP