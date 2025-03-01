//
//  Author: tagrinnell
//  2/18/25
//

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>

#include "JSONLoader.hpp"
#include "Texture.hpp"

class ResourceManager {
public:
    ResourceManager(const char *resourceFilepath);
    ~ResourceManager() = default;

    // Returns a FileDescriptor from a string.  If the resource hasn't been opened, then it needs to open the file
    std::weak_ptr<Texture> getResource(const char *filepath);

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> _fileDirectory;

};

#endif // RESOURCEMANAGER_HPP