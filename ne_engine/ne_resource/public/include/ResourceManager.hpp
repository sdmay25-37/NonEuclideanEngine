//
//  Author: tagrinnell
//  2/18/25
//

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>
#include <utility>

#include "JSONLoader.hpp"
#include "Texture.hpp"

class ResourceManager {
public:
    ResourceManager(const char *resourceFilepath);
    ~ResourceManager() = default;

    // TODO: use result type
    template<typename T>
    std::weak_ptr<T> getResource(const char *filepath) {

        auto it = _fileDirectory.find(filepath);
        if(it != _fileDirectory.end()) {
            auto resource = std::dynamic_pointer_cast<T>(it->second);
            if(resource) {
                return resource;
            }
        }

        // Return empty pointer if the file doesn't exist
        return {};
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> _fileDirectory;

};

#endif // RESOURCEMANAGER_HPP