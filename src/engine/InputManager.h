//
//  Author: Tasman Grinnell
//  Date: 10/30/24
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Input.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class InputManager {
public:
    InputManager(GLFWwindow *window);
    
    // Changing Key/Mouse Bindings
    // currently using ints for associating with action, prolly change later
    bool setContextInput(short int contextId, int key, int actiondId); 
    bool setContextInput(short int contextId, std::vector<int> keys, std::vector<int> actions); // Set inputs based on vectors
    bool setContextInput(std::string xmlFilePath); // Set based on XML file

    // Get action from Inputs.
    bool getAction(short int contextId, int key);

private:
    GLFWwindow* _window;

    // Map of Contexts to Inputs --> Idea is that we find the active context first, 
    // then find the action, then return.
    std::unordered_map<short int, Input> _context_input_map;
};

#endif; // INPUTMANAGER_H