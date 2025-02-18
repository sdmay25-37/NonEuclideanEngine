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

#include "Input.hpp"
#include "Sprite.hpp"
#include "Animation.hpp"

class ResourceManager {
public:
    ResourceManager();

    void initResourceManager();

    // Getters
    Input getInput() { return _input; };

    // Setters
    void setInput(Input newInput) { _input = newInput; };

private:
    Input _input;

};

#endif // RESOURCEMANAGER_HPP