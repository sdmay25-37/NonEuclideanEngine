//
//  Author: Tasman Grinnell
//  2/2/2025
//

#include "Tile.hpp"

Tile::Tile()
{
}
Tile::Tile(int id) : _tileId(id) {}

// Used for testing
std::string Tile::to_string() const
{
    std::ostringstream oss;
    oss << "Tile ID: " << static_cast<int>(_tileId) << "\n"
        << "Sprite ID: " << static_cast<int>(_spriteId) << "\n"
        << "Tile Type: " << static_cast<std::string>(_tileType) << "\n"
        << "Neighbors - Up: " << static_cast<int>(_upTileId) << ", Down: " << static_cast<int>(_downTileId)
        << ", Left: " << static_cast<int>(_leftTileId) << ", Right: " << static_cast<int>(_rightTileId) << "\n"
        << ",World Position: (" << static_cast<int>(worldPosition.first) << "," << static_cast<int>(worldPosition.second) << ")\n"
        << ", Distance from Current Tile: " << static_cast<int>(relationToCurrentTile) << "\n"
        << "Properties: ";

    for (const auto &prop : _properties)
    {
        oss << prop << " ";
    }
    oss << "\n";
    return oss.str();
}