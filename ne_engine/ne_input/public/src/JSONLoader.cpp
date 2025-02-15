//
//  Author: Tasman Grinnell
//  2/2/2025
//

#include "JSONLoader.hpp"

#include <fstream>
#include <iostream>

static void initializeMaps();

JSONLoader::JSONLoader() {
    initializeMaps();
}

JSONLoader::JSONLoader(std::string filepath) {
    initializeMaps();
    _filepath = filepath;
}

// Use for processing a single file
// E.g. one file has one set of bindings (or is 1 json object)
std::vector <std::pair <std::string, int>> JSONLoader::processFile() {
    std::ifstream f (_filepath);

    nlohmann::json jsonData = nlohmann::json::parse(f);

    if (jsonData.size() == 0) {
        // JSON File is empty
        std::cerr << "JSON File empty" << std::endl;
        throw std::length_error("JSON File empty");
    }

    if (jsonData["actions"].size() != jsonData["keys"].size()) {
        std::cerr << "Action and Key arrays are not of equivalent size" << std::endl;
        throw std::length_error("Mismatching action/key sizes");
    }

    std::vector <std::pair <std::string, int>> action_key_bindings;

    for (int i = 0; i < jsonData["actions"].size(); i++) {
        std::pair bind((std::string) (jsonData["actions"].at(i)), _stringToEnum.find(jsonData["keys"].at(i))->second);
        action_key_bindings.push_back(bind);

    }

    return action_key_bindings;
}

std::vector <std::vector <std::pair <std::string, int>>> JSONLoader::processFileArray() {
    std::ifstream f (_filepath);

    nlohmann::json jsonData = nlohmann::json::parse(f);

    if (jsonData.size() == 0) {
        // JSON File is empty
        std::cerr << "JSON File empty" << std::endl;
        throw std::length_error("JSON File empty");
    }

    std::vector <std::vector <std::pair <std::string, int>>> action_key_bindings;

    for (int j = 0; j < jsonData.size(); j++) {
        nlohmann::json single_set = jsonData.at(j);

        if (single_set["actions"].size() != single_set["keys"].size()) {
            std::cerr << "Action and Key arrays are not of equivalent size" << std::endl;
            throw std::length_error("Mismatching action/key sizes");
        }

        std::vector<std::pair<std::string, int>> single_binding;

        for (int i = 0; i < single_set["actions"].size(); i++) {
            std::pair bind((std::string)(single_set["actions"].at(i)), _stringToEnum.find(single_set["keys"].at(i))->second);
            single_binding.push_back(bind);
        }

        action_key_bindings.push_back(single_binding);
    }

    return action_key_bindings;
}

void JSONLoader::outputBindings(std::vector <std::pair <std::string, int>> bindings) {
    nlohmann::json output;

    std::vector <std::string> actions;
    std::vector <std::string> keys;

    for (int i = 0; i < bindings.size(); i++) {
        actions.push_back(bindings.at(i).first);
        keys.push_back(_enumToString.find(bindings.at(i).second)->second);
    }

    output["actions"] = actions;
    output["keys"] = keys;

    std::ofstream out(_filepath);
    out << std::setw(4) << output << std::endl;
}

void JSONLoader::outputBindingsArray(std::vector <std::vector <std::pair <std::string, int>>> bindings) {
    nlohmann::json output;

    for (int i = 0; i < bindings.size(); i++) {
        nlohmann::json obj;

        std::vector <std::string> actions;
        std::vector <std::string> keys;

        for (int j = 0; j < bindings.at(i).size(); j++) {
            actions.push_back(bindings.at(i).at(j).first);
            keys.push_back(_enumToString.find(bindings.at(i).at(j).second)->second);
        }

        obj["actions"] = actions;
        obj["keys"] = keys;

        output.push_back(obj);
    }

    std::ofstream out(_filepath);
    out << std::setw(4) << output << std::endl;
}

// Ignore this abysmal coding, I have no other idea as to how to do this.
void JSONLoader::initializeMaps() {
    // Initialize maps from strings to enums
    _stringToEnum = {
        {"SPACE",  32},
        {"APOSTROPHE", 39},
        {"COMMA", 44},
        {"MINUS", 45},
        {"PERIOD", 46},
        {"SLASH", 47},
        {"0", 48},
        {"1", 49},
        {"2", 50},
        {"3", 51},
        {"4", 52},
        {"5", 53},
        {"6", 54},
        {"7", 55},
        {"8", 56},
        {"9", 57},
        {"SEMICOLON", 59},
        {"EQUAL", 61},
        {"A", 65},
        {"B", 66},
        {"C", 67},
        {"D", 68},
        {"E", 69},
        {"F", 70},
        {"G", 71},
        {"H", 72},
        {"I", 73},
        {"J", 74},
        {"K", 75},
        {"L", 76},
        {"M", 77},
        {"N", 78},
        {"O", 79},
        {"P", 80},
        {"Q", 81},
        {"R", 82},
        {"S", 83},
        {"T", 84},
        {"U", 85},
        {"V", 86},
        {"W", 87},
        {"X", 88},
        {"Y", 89},
        {"Z", 90},
        {"LEFT_BRACKET", 91},
        {"BACKSLASH", 92},
        {"RIGHT_BRACKET", 93},
        {"GRAVE_ACCENT", 96},
        {"ESCAPE", 256},
        {"ENTER", 257},
        {"TAB", 258},
        {"BACKSPACE", 259},
        {"INSERT", 260},
        {"DELETE", 261},
        {"RIGHT", 262},
        {"LEFT", 263},
        {"DOWN", 264},
        {"UP", 265},
        {"PAGE_UP", 266},
        {"PAGE_DOWN", 267},
        {"HOME", 268},
        {"END", 269},
        {"CAPS_LOCK", 280},
        {"SCROLL_LOCK", 281},
        {"NUM_LOCK", 282},
        {"PRINT_SCREEN", 283},
        {"PAUSE", 284},
        {"F1", 290},
        {"F2", 291},
        {"F3", 292},
        {"F4", 293},
        {"F5", 294},
        {"F6", 295},
        {"F7", 296},
        {"F8", 297},
        {"F9", 298},
        {"F10", 299},
        {"F11", 300},
        {"F12", 301},
        {"F13", 302},
        {"F14", 303},
        {"F15", 304},
        {"F16", 305},
        {"F17", 306},
        {"F18", 307},
        {"F19", 308},
        {"F20", 309},
        {"F21", 310},
        {"F22", 311},
        {"F23", 312},
        {"F24", 313},
        {"F25", 314},
        {"KP_0", 320},
        {"KP_1", 321},
        {"KP_2", 322},
        {"KP_3", 323},
        {"KP_4", 324},
        {"KP_5", 325},
        {"KP_6", 326},
        {"KP_7", 327},
        {"KP_8", 328},
        {"KP_9", 329},
        {"KP_DECIMAL", 330},
        {"KP_DIVIDE", 331},
        {"KP_MULTIPLY", 332},
        {"KP_SUBTRACT", 333},
        {"KP_ADD", 334},
        {"KP_ENTER", 335},
        {"KP_EQUAL", 336},
        {"LEFT_SHIFT", 340},
        {"LEFT_CONTROL", 341},
        {"LEFT_ALT", 342},
        {"LEFT_SUPER", 343},
        {"RIGHT_SHIFT", 344},
        {"RIGHT_CONTROL", 345},
        {"RIGHT_ALT", 346},
        {"MOUSE_BUTTON_1", 0},
        {"MOUSE_BUTTON_2", 1},
        {"MOUSE_BUTTON_3", 2},
        {"MOUSE_BUTTON_4", 3},
        {"MOUSE_BUTTON_5", 4},
        {"MOUSE_BUTTON_6", 5},
        {"MOUSE_BUTTON_7", 6},
        {"MOUSE_BUTTON_8", 7},
        {"MOUSE_BUTTON_LEFT", 0},
        {"MOUSE_BUTTON_RIGHT", 1},
        {"MOUSE_BUTTON_MIDDLE", 2}
    };

    _enumToString = {
        {32, "SPACE"},
        {39, "APOSTROPHE"},
        {44, "COMMA"},
        {45, "MINUS"},
        {46, "PERIOD"},
        {47, "SLASH"},
        {48, "0"},
        {49, "1"},
        {50, "2"},
        {51, "3"},
        {52, "4"},
        {53, "5"},
        {54, "6"},
        {55, "7"},
        {56, "8"},
        {57, "9"},
        {59, "SEMICOLON"},
        {61, "EQUAL"},
        {65, "A"},
        {66, "B"},
        {67, "C"},
        {68, "D"},
        {69, "E"},
        {70, "F"},
        {71, "G"},
        {72, "H"},
        {73, "I"},
        {74, "J"},
        {75, "K"},
        {76, "L"},
        {77, "M"},
        {78, "N"},
        {79, "O"},
        {80, "P"},
        {81, "Q"},
        {82, "R"},
        {83, "S"},
        {84, "T"},
        {85, "U"},
        {86, "V"},
        {87, "W"},
        {88, "X"},
        {89, "Y"},
        {90, "Z"},
        {91, "LEFT_BRACKET"},
        {92, "BACKSLASH"},
        {93, "RIGHT_BRACKET"},
        {96, "GRAVE_ACCENT"},
        {256, "ESCAPE"},
        {257, "ENTER"},
        {258, "TAB"},
        {259, "BACKSPACE"},
        {260, "INSERT"},
        {261, "DELETE"},
        {262, "RIGHT"},
        {263, "LEFT"},
        {264, "DOWN"},
        {265, "UP"},
        {266, "PAGE_UP"},
        {267, "PAGE_DOWN"},
        {268, "HOME"},
        {269, "END"},
        {280, "CAPS_LOCK"},
        {281, "SCROLL_LOCK"},
        {282, "NUM_LOCK"},
        {283, "PRINT_SCREEN"},
        {284, "PAUSE"},
        {290, "F1"},
        {291, "F2"},
        {292, "F3"},
        {293, "F4"},
        {294, "F5"},
        {295, "F6"},
        {296, "F7"},
        {297, "F8"},
        {298, "F9"},
        {299, "F10"},
        {300, "F11"},
        {301, "F12"},
        {302, "F13"},
        {303, "F14"},
        {304, "F15"},
        {305, "F16"},
        {306, "F17"},
        {307, "F18"},
        {308, "F19"},
        {309, "F20"},
        {310, "F21"},
        {311, "F22"},
        {312, "F23"},
        {313, "F24"},
        {314, "F25"},
        {320, "KP_0"},
        {321, "KP_1"},
        {322, "KP_2"},
        {323, "KP_3"},
        {324, "KP_4"},
        {325, "KP_5"},
        {326, "KP_6"},
        {327, "KP_7"},
        {328, "KP_8"},
        {329, "KP_9"},
        {330, "KP_DECIMAL"},
        {331, "KP_DIVIDE"},
        {332, "KP_MULTIPLY"},
        {333, "KP_SUBTRACT"},
        {334, "KP_ADD"},
        {335, "KP_ENTER"},
        {336, "KP_EQUAL"},
        {340, "LEFT_SHIFT"},
        {341, "LEFT_CONTROL"},
        {342, "LEFT_ALT"},
        {343, "LEFT_SUPER"},
        {344, "RIGHT_SHIFT"},
        {345, "RIGHT_CONTROL"},
        {346, "RIGHT_ALT"},
        {0, "MOUSE_BUTTON_1"},
        {1, "MOUSE_BUTTON_2"},
        {2, "MOUSE_BUTTON_3"},
        {3, "MOUSE_BUTTON_4"},
        {4, "MOUSE_BUTTON_5"},
        {5, "MOUSE_BUTTON_6"},
        {6, "MOUSE_BUTTON_7"},
        {7, "MOUSE_BUTTON_8"},
        {0, "MOUSE_BUTTON_LEFT"},
        {1, "MOUSE_BUTTON_RIGHT"},
        {2, "MOUSE_BUTTON_MIDDLE"}
    };
}