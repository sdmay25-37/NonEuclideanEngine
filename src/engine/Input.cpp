//
// Created by benj2003 on 10/26/2024.
//

#include "Input.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Input::Input(GLFWwindow *window) {
	_window = window;
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
}

Input::Input(GLFWwindow *window, uint8_t contextId) {
	_window = window;
	_contextId = contextId;
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
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

void Input::bindKeyPress(const std::string &action, int key, const ActionCallback& callback) {
	_action_callback_map[action].push_back(callback);
	_key_action_map[key] = action;
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto action_it = _key_action_map.find(key);
	if(action_it == _key_action_map.end() || action != GLFW_PRESS) return;

	auto callback_it = _action_callback_map.find(action_it->second);
	if(callback_it == _action_callback_map.end()) return;

	for(const auto& callback : callback_it->second) {
        callback();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
	if(!input) return;
	input->keyCallback(window, key, scancode, action, mods);
}