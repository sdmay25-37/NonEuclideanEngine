//
//  Author: tagrinnell
//  2/2/2025
//

/*
    TODO : Think about maybe removing some of the extra methods (like the first outputBindings for example => Will we need it really?)
*/

#ifndef JSONLOADER_HPP
#define JSONLOADER_HPP

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

    std::string getFilepath() { return _filepath; };

private:
	std::unordered_map <int, std::string> _enumToString;
    std::unordered_map <std::string, int> _stringToEnum;
    std::string _filepath;

    void initializeMaps();
};



#endif //JSONLOADER_HPP
