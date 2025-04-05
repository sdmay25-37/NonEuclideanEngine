#include <iostream>

#include "ne_plugin/window/WindowPlugin.hpp"
#include <glad/glad.h>

void WindowPlugin::WindowSetup(Resource<Window> window) {


}

void WindowPlugin::WindowUpdate() {
	glfwPollEvents();
}

void WindowPlugin::WindowPostRender(Resource<Window> window) {
	glfwSwapBuffers(window->GetHandle());
}
