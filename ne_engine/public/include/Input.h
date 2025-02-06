#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

class Input {
public:
	using ActionCallback = std::function<void()>;

	Input();
	Input(GLFWwindow* window);

	// Keyboard
	bool isKeyPressed(int key);
	bool isKeyReleased(int key) const;

	// Mouse
	bool isMouseButtonPressed(int button) const;
	void getMousePosition(double& x, double& y) const;

	// Bind Action, key, and Callback in 1 call
	void bindKeyPress(const std::string& action, int key, const ActionCallback& callback);
	// Bind Action to a key
	void bindKeyPress(const std::string& action, int key);
	// Bind Action to a callback
	void bindKeyPress(const std::string& action, const ActionCallback& callback);
	// Bind actions to keys in a list
	void bindKeyPress(std::vector<std::pair <std::string, int>> bindings, int newBinding);

	// Add a list of bindings to _binding_contexts
	void bindContexts(std::vector<std::vector<std::pair <std::string, int>>> bindings);

	void switchBindings(); // int targetContext);

	std::vector<std::pair <std::string, int>> getBindings();

	std::pmr::unordered_map<int, std::string> getKeyActionMap();

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* _window;

	int _currentContext;

	// An Array of bindings that can be switched (e.g. _binding_contexts[0] is a set that's different from _binding_contexts[1])
	std::vector <std::vector <std::pair <std::string, int>>> _binding_contexts;

	std::unordered_map<std::string, std::vector<ActionCallback>> _action_callback_map;
	std::pmr::unordered_map<int, std::string> _key_action_map;
};



#endif //INPUTHANDLER_H
