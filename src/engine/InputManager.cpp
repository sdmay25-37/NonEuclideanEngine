//
//  Author: Tasman Grinnell
//  Date: 10/30/24
//

#include "InputManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

/*
Rambling Notes:
    When we're gathering input, the input manager will likely also dispatch the actions
    that's associated with each of the contexts
    so the access is 
    key press --> context access --> key mapping --> actionw
    .                                                   |
    Return action associated <--------------------------|
    Currently, we can probably handle this in the Input Manager class itself

    Try to store states for polling (game design team queries state)
*/

//
//  PROTOTYPES
//
bool contextExists (int contextId, std::unordered_map<short int, Input> map);
void initializeMaps(std::unordered_map <int, std::string> _enumToString, std::unordered_map <std::string, int> _stringToEnum);
std::vector<Input> parseJSON(std::string filepath);

// Initialize --> for each of the Inputs, we'll have to create 
InputManager::InputManager(GLFWwindow *window) {
    _window = window;
    _configurations = nlohmann::json::array();
    initializeMaps(_enumToString, _stringToEnum);
}

bool InputManager::setContextInput(short int contextId, int key, std::string action, const ActionCallback& callback) {
    Input newInput(_window);

    if (!contextExists(contextId, _context_input_map)) {
        _context_input_map[contextId] = newInput;
    } else {
        Input newInput = _context_input_map[contextId];    // Fix eventually
    }

    newInput.bindKeyPress(action, key, callback);
    return true;
}

// Set inputs based on vectors --> This is going to be either batch input during 
// startup or flushing more than 1 changed key bindings (e.g. changing a bunch of callbacks in menu)
bool InputManager::setContextInput(short int contextId, std::vector<int> keys, std::vector<std::string> actions, std::vector<const ActionCallback&> callback) {
    if (keys.size() != actions.size() || keys.size() != callback.size()) return false;

    Input newInput(_window);

    if (!contextExists(contextId, _context_input_map)) {
        _context_input_map[contextId] = newInput;
    } else {
        newInput = _context_input_map[contextId];    // Fix eventually
    }

    for (int i = 0; i < keys.size(); i++) {
        newInput.bindKeyPress(actions.at(i), keys.at(i), callback.at(i));
    }

    return true;
}

// Set based on JSON file
bool InputManager::setContextInput() {
    nlohmann::json configurations = processJSON(_jsonFilePath);

    for (int i = 0; i < configurations.size(); i++) {sdad
        nlohmann::json obj = configurations[i];
        
        // Grab Fields for each context
        uint8_t contextId = obj["contextId"];                           // TODO: TBD, maybe use strings instead?
        int priority = obj["priority"];
        std::vector<std::string> keys = obj["keys"];
        std::vector<std::string> actions = obj["actions"];
        // std::vector<std::string> callbacks = obj["ActionCallbacks"];
        
        // Create Input class to bind to ContextID
        Input newIn(_window, contextId, priority);
        for (int j = 0; j < keys.size(); j++) {
            // For now, we just bind the key to the action string, need to figure out how to link this later
            newIn.bindsKeyPress(actions.at(j), _stringToEnum.find(actions.at(j)));
        }

        _context_input_map.emplace(contextId, newIn);
    } 

    return true;
}                                         

// Get action from Inputs.
bool InputManager::getAction(short int contextId, int key) {
    // return _context_input_map.find(contextId).find();
    return;
}

bool contextExists (int contextId, std::unordered_map<short int, Input> map) {
    return (map.find(contextId) != map.end());
}

// Process configuration information found in JSON file
// Specified as a function to improve readability 
inline nlohmann::json processJSON(std::string filepath) {
    return nlohmann::json::parse(std::ifstream(filepath));
}

// Write configurations to JSON file
void InputManager::flushConfigurations() {
    nlohmann::json outputJson;
    int indexCtr = 0;

    for (Input i : _context_input_map) {
        std::pmr::unordered_map<int, std::string> keyActionMap = i.getKeyActionMap();
        std::vector <std::string> actions;
        std::vector <int> keys;

        for (auto& [key, value] : keyActionMap) {
            actions.push_back(value);
            keys.push_back(key);
        }

        outputJson[indexCtr]["contextId"] = i.contextId;
        outputJson[indexCtr]["priority"] = i.priority;
        outputJson[indexCtr]["actions"] = actions;
        outputJson[indexCtr]["keys"] = keys;

        indexCtr++;
    }

    // Write out to JSON
    std::ofstream out("src/bindings/bindings.json");
    out << std::setw(4) << outputJson << std::endl;
}

// Ignore this abysmall coding, I have no other idea as to how to do this.
void initializeMaps(std::unordered_map <int, std::string> _enumToString, std::unordered_map <std::string, int> _stringToEnum) {
    

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
