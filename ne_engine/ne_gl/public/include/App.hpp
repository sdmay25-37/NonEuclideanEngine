#ifndef APP_HPP
#define APP_HPP

#include <vector>

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Input.hpp"
#include "Render.hpp"
#include "ShaderProgram.hpp"


constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;


class App {
public:
	// App();
	// ~App();

	void run();

private:
	GLFWwindow* _window;
	entt::registry _registry;
	Render* _renderSystem;

	// Temporary testing stuff
	int _count = 0;
	std::vector<glm::vec4> _uvRanges;
	unsigned int _texture;
	int _tileWidth, _tileHeight;
	float _uvWidth, _uvHeight;
	ShaderProgram* _shaders;
	Input* _charInput;


  	void init();
	void update();
	void render();
	void cleanup();

};

#endif //APP_HPP
