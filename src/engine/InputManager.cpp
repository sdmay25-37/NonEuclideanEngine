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
    _configurations = nlohmann::json::array();
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

    for (int i = 0; i < configurations.size(); i++) {
        nlohmann::json obj = configurations[i];
        
        // Grab Fields for each context
        int contextId = obj["contextId"];
        std::vector<std::string> keys = obj["keys"];
        std::vector<std::string> actions = obj["actions"];
        std::vector<std::string> callbacks = obj["ActionCallbacks"];
        
        // Create Input class to bind to ContextID
        Input newIn(_window);
        for (int j = 0; j < keys.size(); j++) {
            
        }

        _context_input_map.emplace(contextId, newIn);
    } 

    return true;
}                                         

// Get action from Inputs.
bool getAction(short int contextId, int key);

bool contextExists (int contextId, std::unordered_map<short int, Input> map) {
    return (map.find(contextId) != map.end());
}

// Process configuration information found in JSON file
inline nlohmann::json processJSON(std::string filepath) {
    return nlohmann::json::parse(std::ifstream(filepath));
}

// Write configurations to JSON file
void flushConfigurations() {

}