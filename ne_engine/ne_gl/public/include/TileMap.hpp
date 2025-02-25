//
//  Author: Tasman Grinnell
//  2/25/2025
//

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <string>

struct Tile {
    uint64_t tileInformation;
    uint8_t tileTexture;
};

class TileMap {
public:
    TileMap();
    TileMap(std::vector<Tile> tileList, std::vector<Tile> seedList);

    void loadTiles(std::string filepath); // Need to change this.  Trying to figure out a bit more about what the goal 
    //of the function is or if we can just pass a vector in for creating the list.

    std::vector <std::pair <std::string, std::string>> getTileInformation();    // Returns a vector of pairs of a <direction string, texture classification>

private: 
    std::vector<Tile> _tileList;
    std::vector<Tile> _seedList;  
};

#endif // TILEMAP_HPP