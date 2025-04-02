//
//  Author: Cory Roth
//  3/14/2025
//

/*
    Change to Tree structure
*/

#ifndef TILE_HPP
#define TILE_HPP

// #include "Quadtree.h"

#include <vector>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

class Tile
{
public:
    Tile();
    Tile(int id);
    bool operator==(const Tile &other) const
    {
        return _tileId == other._tileId;
    }

    int8_t _tileId;
    int8_t _leftTileId;
    int8_t _rightTileId;
    int8_t _upTileId;
    int8_t _downTileId;
    std::string _tileType; // Maybe change into an enum?
    int8_t _spriteId;
    std::vector<std::string> _properties;

    // world position coordinates
    //  0,0 is upper left of global tile map
    // 1,0 is the tile left to tile at 0,0
    // 0,1 is the tile down from tile at 0,0
    //-1,-1 is tile not in map
    std::pair<int8_t, int8_t> worldPosition;

    // Number of tiles away from currentTiles
    //     TODO make sure this is current and if a new currentTile is chosen this either gets updated or store current Tile\\
    //may not be usefull?
    // use worldPosition to do this?

    // Currently only has how many tiles away from currentTile
    int8_t relationToCurrentTile;

    // Used for testing
    std::string to_string() const;

private:
};

#endif // TILEMAP_HPP