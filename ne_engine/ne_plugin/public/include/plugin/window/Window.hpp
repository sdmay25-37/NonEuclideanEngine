#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


struct Window {
	GLFWwindow* ptr = nullptr;
	const int WIDTH = 800;
	const int HEIGHT = 600;
};

class App;





#endif //WINDOW_HPP
