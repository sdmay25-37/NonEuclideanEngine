#ifndef APP_HPP
#define APP_HPP

#include <vector>

#include <entt/entt.hpp>

#include "Input.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "ne_system/Plugin.hpp"
#include "ne_system/Resource.hpp"
#include "ne_system/SystemExecutor.hpp"


class App {
public:
	App() : _executor(nullptr) {}

	void Run();

	App& AddSystems(const ScheduleLabel schedule, SystemSet&& system_set) {
		_schedules[schedule].emplace_back(std::move(system_set));
		return *this;
	}

	template<typename T, typename... Args>
	App& InsertResource(Args&&... args) {
		_resource_manager.Insert<T>(std::forward<Args>(args)...);
		return *this;
	}

	template<typename Base, typename Derived, typename... Args>
	App& InsertResourceBase(Args&&... args) {
		_resource_manager.InsertBase<Base, Derived>(std::forward<Args>(args)...);
		return *this;
	}

	template<typename T>
	App& AddPlugin() {
		static_assert(std::is_base_of_v<Plugin, T>, "T must be a Plugin");

		// Ignore duplicate additions
		if(_plugin_names.find(std::type_index(typeid(T))) != _plugin_names.end()) return *this;

		_plugin_names.insert(std::type_index(typeid(T)));
		std::unique_ptr<Plugin> plugin = std::make_unique<T>();
		plugin->Build(*this);
		return *this;
	}

private:

	entt::registry _registry;
	ResourceManager _resource_manager;

	std::unique_ptr<SystemExecutor> _executor;
	EnumArray<ScheduleLabel, std::vector<SystemSchedule>> _schedules;

	std::unordered_set<std::type_index> _plugin_names;

  	void Startup();
	void Update();
	void Render();
	void Cleanup();

};

#endif //APP_HPP
