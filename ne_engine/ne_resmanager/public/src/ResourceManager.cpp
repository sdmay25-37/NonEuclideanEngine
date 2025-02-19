//
//  Author: tagrinnell
//  2/18/25
//

#include "ResourceManager.hpp"

ResourceManager::ResourceManager () {
}

ResourceManager::ResourceManager(Input input, GLFWwindow *window) {
    _input = input;
    _window = window;
    _loader;
}

void ResourceManager::outputBindings() {
    _loader.outputBindingsArray(_input.getBindingsArray());
}

void ResourceManager::initResourceManager(GLFWwindow *window, const char* bindings) {
    // Initialize Private Variables
    _window = window;
    Input _input(_window);
    JSONLoader _loader(bindings);
    
    auto processedBindings = _loader.processFileArray();
    _input.bindContexts(processedBindings);
}