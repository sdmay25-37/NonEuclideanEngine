#include <iostream>

#include "ne_plugin/window/WindowPlugin.hpp"
#include <glad/glad.h>

void WindowPlugin::WindowUpdate() {
	glfwPollEvents();
}

void WindowPlugin::WindowPostRender(Resource<Window> window) {
	window->SwapBuffers();
}
