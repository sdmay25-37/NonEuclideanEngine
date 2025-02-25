//
//  Author: Tasman Grinnell
//  2/2/2025
//

#include "TileMap.hpp"

TileMap::TileMap() {


}

TileMap::TileMap(std::vector<Tile> tileList, std::vector<Tile> seedList) {

}

void TileMap::loadTiles(std::string filepath) {

}

std::vector <std::pair <std::string, std::string>> TileMap::getTileInformation() {
    std::vector <std::pair <std::string, std::string>> returnTiles;
    for (int i = 0; i < _tileList.size(); i++) {
        Tile currTile = _tileList.at(i);
        
        
    }

    return returnTiles;
}    // Returns a vector of pairs of a <direction string, texture classification>