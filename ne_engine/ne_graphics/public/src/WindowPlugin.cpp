#include <iostream>

#include "WindowPlugin.hpp"
#include <glad/glad.h>

void WindowPlugin::WindowSetup(Resource<Window> window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window->ptr = glfwCreateWindow(window->WIDTH, window->HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if(window->ptr == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window->ptr);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glfwSetFramebufferSizeCallback(window->ptr, [] (GLFWwindow* _, const int w, const int h) {
		glViewport(0, 0, w, h);
	});
}

void WindowPlugin::WindowUpdate() {
	glfwPollEvents();
}

void WindowPlugin::WindowPostRender(Resource<Window> window) {
	glfwSwapBuffers(window->ptr);
}
