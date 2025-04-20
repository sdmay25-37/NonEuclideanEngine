//
//  Author: Cory Roth
//  3/14/2025
//
#ifndef TILE_HPP
#define TILE_HPP

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

    int _tileId;
    int _leftTileId;
    int _rightTileId;
    int _upTileId;
    int _downTileId;
    std::string _tileType; // Maybe change into an enum? also not used
    int _spriteId;         // Not used
    std::string sprite;
    std::vector<std::string> _properties; // Not Used

    // world position coordinates
    std::pair<int, int> worldPosition;

    // Currently only has how many tiles away from currentTile
    int8_t relationToCurrentTile;
    // IDK how useful this is?
    std::pair<int, int> relationMappingToCurrentTile;

    // Used for testing
    std::string to_string() const;

private:
};

#endif // TILEMAP_HPP