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
#include <functional>

class InputManager {
public:
	using ActionCallback = std::function<void()>;

    InputManager();
    InputManager(GLFWwindow *window);
    InputManager(GLFWwindow *window, std::string filepath);

    // Changing Key/Mouse Bindings
    // currently using ints for associating with action, prolly change later
    bool setContextInput(int contextId, int key, std::string action, const ActionCallback& callback);
    // bool setContextInput(int contextId, std::vector<int> keys, std::vector<std::string> actions); // Set inputs based on vectors
    bool setContextInput(int contextId, std::vector<int> keys, std::vector<std::string> actions, std::vector<ActionCallback> callback); // Set inputs based on vectors
    bool setContextInput(); // Set based on XML file

    std::unordered_map<int, Input> getContextInputMap();

    void flushConfigurations(); 
    // Get action from Inputs.
    bool getAction(int contextId, int key);

private:
    GLFWwindow* _window;
    nlohmann::json _configurations;
    std::string _jsonFilePath;

    std::unordered_map <int, std::string> _enumToString;
    std::unordered_map <std::string, int> _stringToEnum;

    // Map of Contexts to Inputs --> Idea is that we find the active context first, 
    // then find the action, then return.
    std::unordered_map<int, Input> _context_input_map;
};

#endif // INPUTMANAGER_H