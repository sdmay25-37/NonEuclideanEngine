//
//  Author: tagrinnell
//  2/18/25
//

/*

Resource Manager Scope
---------------------------------------------
ShaderProgram
Sprite
    Animations
ECS
Render Subsystem
Input
    JSONLoader
---------------------------------------------

*/

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <iostream>

#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "JSONLoader.hpp"
#include "ShaderProgram.hpp"

struct FileInfo {
    int numberOfReferences;         // Keep track of all the references to the given resource
    unsigned char* spritesheet;
};

class ResourceManager {
public:
    ResourceManager();
    ResourceManager(std::string bindingsFilePath);

    void initResourceManager();
    void outputBindings(std::vector <std::vector <std::pair <std::string, int>>> bindings);

    int retireResource(std::string filepath);

    // Returns a FileDescriptor from a string.  If the resource hasn't been opened, then it needs to open the file
    unsigned char* getResource(const char* filepath);

    // Getters

    // Setters

private:
    JSONLoader _loader;
    std::unordered_map <const char*, FileInfo> _fileDirectory;

};

#endif // RESOURCEMANAGER_HPP