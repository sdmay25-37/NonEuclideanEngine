#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>

class Input {
public:
	Input(GLFWwindow* window);

	// Keyboard
	bool isKeyPressed(int key) const;
	bool isKeyReleased(int key) const;

	// Mouse
	bool isMouseButtonPressed(int button) const;
	void getMousePosition(double& x, double& y) const;

private:
  GLFWwindow* _window;
};



#endif //INPUTHANDLER_H
