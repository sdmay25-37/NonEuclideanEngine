//
// Created by benj2003 on 10/26/2024.
//

#include "Input.h"

Input::Input(GLFWwindow *window) {
	_window = window;
}

bool Input::isKeyPressed(int key) const {
	return glfwGetKey(_window, key) == GLFW_PRESS;
}

bool Input::isKeyReleased(int key) const {
	return glfwGetKey(_window, key) == GLFW_RELEASE;
}


bool Input::isMouseButtonPressed(int button) const {
	return glfwGetMouseButton(_window, button) == GLFW_PRESS;
}

void Input::getMousePosition(double &x, double &y) const {
	glfwGetCursorPos(_window, &x, &y);
}
