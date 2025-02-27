//
//  Author: Tasman Grinnell
//  2/2/2025
//

#include "TileMap.hpp"

TileMap::TileMap() {


}

TileMap::TileMap(std::vector<Tile> tileList, std::vector<Tile> seedList) {
    _tileList = tileList;
    _seedList = seedList;
}

// TODO: Recreate
void TileMap::loadTiles(std::string filepath) {

}

std::vector <std::pair <char*, std::string>> TileMap::getTileInformation() {
    std::vector <std::pair <char*, std::string>> returnTiles;
    for (int i = 0; i < _tileList.size(); i++) {
        Tile currTile = _tileList.at(i);
        
        int end = currTile.tileLocation & 0xF800000000000000;   // Mask the top 5 bits for the length
        char* dirString[end];   // Create a string of size end, which is max 30 characters 
        for (int j = 0; j < end; j++) {
            
        }

    }

    return returnTiles;
}    // Returns a vector of pairs of a <direction string, texture classification>