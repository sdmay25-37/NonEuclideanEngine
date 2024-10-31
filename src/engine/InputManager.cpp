//
//  Author: Tasman Grinnell
//  Date: 10/30/24
//

#include "InputManager.h"

/*
Rambling Notes:
    When we're gathering input, the input manager will likely also dispatch the actions
    that's associated with each of the contexts
    so the access is 
    key press --> context access --> key mapping --> action
.                                                       |
    Return action associated <--------------------------|
    Currently, we can probably handle this in the Input Manager class itself
*/

// Initialize --> for each of the Inputs, we'll have to create 
InputManager::InputManager(GLFWwindow *window) {
    _window = window;
}

bool InputManager::setContextInput(short int contextId, int key, int actiondId) {
    return true;
}

// Set inputs based on vectors
bool InputManager::setContextInput(short int contextId, std::vector<int> keys, std::vector<int> actions) {
    if (keys.size() != actions.size()) return false;


    
    return true;
}

// Set based on XML file
bool InputManager::setContextInput(std::string xmlFilePath) {
    return true;
}                                         

// Get action from Inputs.
bool getAction(short int contextId, int key);

bool contextExists (int contextId) {
    return _context_input_map.find(contextId) != NULL;
}