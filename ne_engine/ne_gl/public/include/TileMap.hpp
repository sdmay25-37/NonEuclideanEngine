//
//  Author: Tasman Grinnell
//  2/25/2025
//

#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>

struct Tile {
    long long int tileInformation;
};

class TileMap {
public:
    TileMap();

    void loadTiles(std::string );

private: 
    std::vector <Tile> _tileList;
    std::vector <Tile> _seedList;
};

#endif // TILEMAP_HPP