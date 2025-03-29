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

    int8_t _tileId;
    int8_t _leftTileId;
    int8_t _rightTileId;
    int8_t _upTileId;
    int8_t _downTileId;
    std::string _tileType; // Maybe change into an enum?
    int8_t _spriteId;
    std::vector<std::string> _properties;

    // Used for testing
    std::string to_string() const;

private:
};

#endif // TILEMAP_HPP