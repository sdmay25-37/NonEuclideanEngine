#include <iostream>
#include <glad/glad.h>

#include "ne_plugin/window/Window.hpp"



Window::Window(unsigned int width, unsigned int height)
	: _width(width), _height(height) {

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	_ptr = glfwCreateWindow(width, height, "Non-Euclidean Engine", nullptr, nullptr);
	if(_ptr == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_ptr);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glfwSetFramebufferSizeCallback(_ptr, [] (GLFWwindow* _, const int w, const int h) {
		glViewport(0, 0, w, h);
	});
}

Window::~Window() {
	if(_ptr != nullptr) glfwDestroyWindow(_ptr);
	glfwTerminate();
}
