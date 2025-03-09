

#include "App.hpp"

#include <iostream>
#include <GLFW/glfw3.h>

constexpr int UPDATES_PER_SECOND = 60;
constexpr double SECONDS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
constexpr int MAX_FRAMESKIP = 5;

void App::run() {

	GLFWwindow* window;

	init();

	// TESTING
	unsigned long updates = 0;
	double startTime = glfwGetTime();

	double last_time = glfwGetTime();
	double acc = 0.0;
	int frames_skipped = 0;

	while (!glfwWindowShouldClose(window)) {
		double current_time = glfwGetTime();
		double delta_time = current_time - last_time;

		last_time = current_time;
		acc += delta_time;

		while(acc > SECONDS_PER_UPDATE && frames_skipped < MAX_FRAMESKIP) {
			update();
			updates++;
			acc -= SECONDS_PER_UPDATE;
			frames_skipped++;
		}

		// Only render if at least one update has occurred
		if(frames_skipped > 0) {
			render();
			frames_skipped = 0;
			std::cout << "UPS: " << updates / (current_time - startTime) << std::endl;
		}

	}

	cleanup();
}