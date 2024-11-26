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
	Input(GLFWwindow* window, uint8_t contextId);
	Input(GLFWwindow* window, uint8_t contextId, int priority);

	// Keyboard
	bool isKeyPressed(int key) const;
	bool isKeyReleased(int key) const;

	// Mouse
	bool isMouseButtonPressed(int button) const;
	void getMousePosition(double& x, double& y) const;

	void bindKeyPress(const std::string& action, int key, const ActionCallback& callback);
	void bindKeyPress(const std::string& action, int key);

	std::pmr::unordered_map<int, std::string> getKeyActionMap();

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	uint8_t getContextId();
	int getPriority();


private:
	GLFWwindow* _window;
	uint8_t _contextId;
	int _priority;

	std::unordered_map<std::string, std::vector<ActionCallback>> _action_callback_map;

	std::pmr::unordered_map<int, std::string> _key_action_map;
};



#endif //INPUTHANDLER_H
