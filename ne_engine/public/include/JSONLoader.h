//
//  Author: tagrinnell
//  2/2/2025
//

#ifndef JSONLOADER_H
#define JSONLOADER_H

#include <nlohmann/json.hpp>

#include <vector>
#include <string>

class JSONLoader {
public:

    JSONLoader();
    JSONLoader(std::string filepath);

    std::vector <std::pair <std::string, int>> processFile();
    std::vector <std::vector <std::pair <std::string, int>>> processFileArray();
    
    void outputBindings(std::vector <std::pair <std::string, int>> bindings);
    void outputBindingsArray(std::vector <std::vector <std::pair <std::string, int>>> bindings);

private:
	std::unordered_map <int, std::string> _enumToString;
    std::unordered_map <std::string, int> _stringToEnum;
    std::string _filepath;

    void initializeMaps();
};



#endif //JSONLOADER_H
