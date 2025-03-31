#ifndef RENDERPLUGIN_HPP
#define RENDERPLUGIN_HPP

#include "App.hpp"
#include "ne_system/Plugin.hpp"

class RenderPlugin final : public Plugin {
public:
	void Build(App &app) override {
		app
			.InsertResource<Renderer>()
			.AddSystems(ScheduleLabel::STARTUP, SystemSet(Setup))
			.AddSystems(ScheduleLabel::RENDER, SystemSet(Render));
	}

	static void Setup(Resource<Renderer> renderer);
	static void Render(entt::registry& registry, Resource<Renderer> renderer);
};



#endif //RENDERPLUGIN_HPP
