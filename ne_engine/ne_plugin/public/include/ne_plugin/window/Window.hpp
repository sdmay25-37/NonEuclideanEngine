#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>



// TODO: I don't like how the Window contains the entire GLFW context, but OpenGL is stupid
class Window {
public:

	Window(unsigned int width, unsigned int height);
	~Window();

	[[nodiscard]] unsigned int GetWidth() const { return _width; }
	[[nodiscard]] unsigned int GetHeight() const { return _height; }
	[[nodiscard]] GLFWwindow* GetHandle() const { return _ptr; }

private:
	GLFWwindow* _ptr;
	unsigned int _width, _height;
};





#endif //WINDOW_HPP
