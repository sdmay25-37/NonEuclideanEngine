

#include "App.hpp"

#include <GLFW/glfw3.h>

const int TICKS_PER_SECOND = 60;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

void App::run() {

	GLFWwindow* window;

	init();

	while (!glfwWindowShouldClose(window)) {
		update();
		render();
	}

	cleanup();
}