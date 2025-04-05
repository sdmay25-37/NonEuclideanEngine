#ifndef WINDOW_HPP
#define WINDOW_HPP


// TODO: I don't like how the Window contains the entire GLFW context, but OpenGL is stupid
class Window {
public:

	Window(unsigned int width, unsigned int height)
		: _width(width), _height(height) {}

	virtual ~Window() = default;

	virtual void PollEvents() = 0;
	virtual void SwapBuffers() = 0;
	virtual bool ShouldClose() = 0;

	[[nodiscard]] unsigned int GetWidth() const { return _width; }
	[[nodiscard]] unsigned int GetHeight() const { return _height; }

private:
	unsigned int _width, _height;
};


#endif //WINDOW_HPP
