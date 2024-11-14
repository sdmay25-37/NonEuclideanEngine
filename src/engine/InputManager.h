//
//  Author: Tasman Grinnell
//  Date: 10/30/24
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Input.h"

#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <string>

class InputManager {
public:
	using ActionCallback = std::function<void()>;

    InputManager(GLFWwindow *window);
    
    // Changing Key/Mouse Bindings
    // currently using ints for associating with action, prolly change later
    bool InputManager::setContextInput(short int contextId, int key, std::string action, const ActionCallback& callback);
    bool InputManager::setContextInput(short int contextId, std::vector<int> keys, std::vector<std::string> actions, std::vector<const ActionCallback&> callback); // Set inputs based on vectors
    bool setContextInput(); // Set based on XML file

    // Get action from Inputs.
    bool getAction(short int contextId, int key);

private:
    GLFWwindow* _window;
    nlohmann::json _configurations;
    std::string _jsonFilePath = "../bindings/example_bindings.json";

    // Map of Contexts to Inputs --> Idea is that we find the active context first, 
    // then find the action, then return.
    std::unordered_map<short int, Input> _context_input_map;
};

#endif; // INPUTMANAGER_H