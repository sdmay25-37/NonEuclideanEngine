#ifndef APP_HPP
#define APP_HPP

#include <vector>

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ne_system/Resource.hpp>
#include <ne_system/SystemExecutor.hpp>

#include "Input.hpp"
#include "Render.hpp"
#include "ShaderProgram.hpp"
#include "TextureManager.hpp"

constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

enum class ScheduleLabel {
	STARTUP, UPDATE,
	MAX_VALUE
};

class App {
public:
	App()
		: _window(nullptr), _renderSystem(nullptr), _executor(nullptr) {
	};
	// ~App();

	void run();

	App& AddSystems(const ScheduleLabel schedule, SystemSet&& system_set) {
		_schedules[schedule] = SystemSchedule(std::move(system_set));
		return *this;
	}

	template<typename T, typename... Args>
	App& InsertResource(Args&&... args) {
		_resource_manager.insert<T>(std::forward<Args>(args)...);
		return *this;
	}

private:
	GLFWwindow* _window;
	entt::registry _registry;
	Render* _renderSystem;

	std::unique_ptr<SystemExecutor> _executor;
	EnumArray<ScheduleLabel, SystemSchedule> _schedules;

	ResourceManager _resource_manager;

	// Temporary testing stuff
	int _count = 0;
	std::vector<glm::vec4> _uvRanges;
	unsigned int _texture;
	int _tileWidth, _tileHeight;
	float _uvWidth, _uvHeight;
	ShaderProgram* _shaders;
	Input* _charInput;
	TextureManager* _texture_manager;


  	void init();
	void update();
	void render();
	void cleanup();

};

#endif //APP_HPP
