//
//  Author: Tasman Grinnell
//  Date: 10/30/24
//

#include "InputManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*
Rambling Notes:
    When we're gathering input, the input manager will likely also dispatch the actions
    that's associated with each of the contexts
    so the access is 
    key press --> context access --> key mapping --> action
    .                                                   |
    Return action associated <--------------------------|
    Currently, we can probably handle this in the Input Manager class itself

    Try to store states for polling (game design team queries state)
*/

//
//  PROTOTYPES
//
bool contextExists (int contextId, std::unordered_map<short int, Input> map);
std::vector<Input> parseJSON(std::string filepath);

// Initialize --> for each of the Inputs, we'll have to create 
InputManager::InputManager(GLFWwindow *window) {
    _window = window;
    _configurations = json::array();
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
bool InputManager::setContextInput(std::string jsonFilePath) {
    try {

    } catch (_exception e) { /* Do Nothing Lmao*/  }

    json configurations = json::parse(std::ifstream(jsonFilePath));

    return true;
}                                         

// Get action from Inputs.
bool getAction(short int contextId, int key);

bool contextExists (int contextId, std::unordered_map<short int, Input> map) {
    return (map.find(contextId) != map.end());
}

// Process configuration information found in JSON file
json processJSON(const char* filepath) {
    return json::parse(std::ifstream(filepath));
}

// Write configurations to JSON file
void flushConfigurations() {

}