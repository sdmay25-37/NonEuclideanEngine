#ifndef GLFWWINDOW_HPP
#define GLFWWINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Window.hpp"

class GLFWWindow : Window {
public:
	GLFWWindow(unsigned int width, unsigned int height);
	~GLFWWindow() override;

	void PollEvents() override { glfwPollEvents(); }
	void SwapBuffers() override { glfwSwapBuffers(_handle); }
	bool ShouldClose() override { return glfwWindowShouldClose(_handle); }

private:
	GLFWwindow* _handle;
};

#endif //GLFWWINDOW_HPP
