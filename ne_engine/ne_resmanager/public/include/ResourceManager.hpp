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


#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <iostream>

#include "JSONLoader.hpp"
#include "ShaderProgram.hpp"

struct FileInfo {
    int numberOfReferences;         // Keep track of all the references to the given resource
    unsigned char* spritesheet;     // Spritesheet image that has been loaded
};

class ResourceManager {
public:
    ResourceManager();
    ResourceManager(std::string bindingsFilePath);

    void initResourceManager();
    void outputBindings(std::vector <std::vector <std::pair <std::string, int>>> bindings);

    void removeReference(const char* filepath);
    void retireResource(const char* filepath);

    // Returns a FileDescriptor from a string.  If the resource hasn't been opened, then it needs to open the file
    unsigned char* getResource(const char* filepath);

    void retireManager();

private:
    JSONLoader _loader;
    std::unordered_map <const char*, FileInfo> _fileDirectory;

};

#endif // RESOURCEMANAGER_HPP