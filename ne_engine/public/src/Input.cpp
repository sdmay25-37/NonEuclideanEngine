//
// Created by benj2003 on 10/26/2024.
//

#include "Input.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Input::Input() {
	_window = NULL;
}

Input::Input(GLFWwindow *window) {
	_window = window;
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
}

bool Input::isKeyPressed(int key) {
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

void Input::bindKeyPress(const std::string& action, int key) {
	int formerKey = 0;

	for (const auto& [oldkey, value] : _key_action_map) {
		if (value == action && oldkey != key) {
			formerKey = oldkey;
		}

	}
	if (formerKey != 0) {
		_key_action_map[key] = _key_action_map[formerKey];
		_key_action_map.erase(formerKey);
	} else {
		_key_action_map[key] = action; 
	}
}

void Input::bindKeyPress(const std::string& action, const ActionCallback& callback) {
	_action_callback_map[action].push_back(callback);
}

void Input::bindKeyPress(std::vector<std::pair <std::string, int>> bindings, int newBinding) {
	if (bindings.size() == 0) {
		std::cerr << "Empty bindings vector." << std::endl;
		return;
	}

	for (int i = 0; i < bindings.size(); i++) {
        this->bindKeyPress(bindings.at(i).first, bindings.at(i).second);
    }
	
	if (newBinding) {
		_binding_contexts.push_back(bindings);
	}
}


void Input::bindContexts(std::vector<std::vector<std::pair <std::string, int>>> bindings) {
	for (int i = 0; i < bindings.size(); i++) {
		_binding_contexts.push_back(bindings.at(i));
	}

	_currentContext = 0;
	bindKeyPress(_binding_contexts.at(0), 0);
}

void Input::switchBindings() {
	std::cout << _currentContext << " " << _currentContext + 1 << std::endl;
	bindKeyPress(_binding_contexts.at((_currentContext + 1) % _binding_contexts.size()), 0);
	_currentContext = (_currentContext + 1) % _binding_contexts.size();
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!action) return; 
	auto action_it = _key_action_map.find(key);
	if(action_it == _key_action_map.end() || action != GLFW_PRESS || action == GLFW_RELEASE) return;

	auto callback_it = _action_callback_map.find(action_it->second);
	if(callback_it == _action_callback_map.end()) return;

	for(const auto& callback : callback_it->second) {
        callback();
    }
}

std::vector<std::pair <std::string, int>> Input::getBindings() {
	std::vector<std::pair <std::string, int>> bindings;

	for (auto element : _key_action_map) {
		bindings.push_back(std::make_pair(element.second, element.first));
	}

	return bindings;
}


std::pmr::unordered_map<int, std::string> Input::getKeyActionMap() {
	return _key_action_map;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	if(!input) return;
	input->keyCallback(window, key, scancode, action, mods);
}