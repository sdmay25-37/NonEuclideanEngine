#ifndef APP_HPP
#define APP_HPP

#include <vector>

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "ne_system/Resource.hpp"
#include "ne_system/SystemExecutor.hpp"


constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;
constexpr float ASPECT_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

enum class ScheduleLabel {
	STARTUP, UPDATE,
	MAX_VALUE
};

class App;

class Plugin {
public:
	virtual void build(App& app) = 0;
	virtual ~Plugin() = default;
};

class App {
public:
	App()
		: _window(nullptr), _renderSystem(nullptr), _executor(nullptr) {};

	void Run();

	App& AddSystems(const ScheduleLabel schedule, SystemSet&& system_set) {
		_schedules[schedule] = SystemSchedule(std::move(system_set));
		return *this;
	}

	template<typename T, typename... Args>
	App& InsertResource(Args&&... args) {
		_resource_manager.insert<T>(std::forward<Args>(args)...);
		return *this;
	}

	template<typename T>
	App& AddPlugin() {
		static_assert(std::is_base_of_v<Plugin, T>, "T must be a Plugin");
		_plugins.emplace_back(std::make_unique<T>());
		return *this;
	}

private:
	GLFWwindow* _window;
	entt::registry _registry;
	Renderer* _renderSystem;

	std::unique_ptr<SystemExecutor> _executor;
	EnumArray<ScheduleLabel, SystemSchedule> _schedules;
	std::vector<std::unique_ptr<Plugin>> _plugins;
	ResourceManager _resource_manager;

	// Temporary testing stuff
	unsigned int _texture;
	ShaderProgram* _shaders;
	Input* _charInput;


  	void Init();
	void Update();
	void Render();
	void Cleanup();

};

#endif //APP_HPP
