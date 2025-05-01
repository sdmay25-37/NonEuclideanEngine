#ifndef RENDERHYPPLUGIN_HPP
#define RENDERHYPPLUGIN_HPP

#include "ne_plugin/window/Window.hpp"

#include "App.hpp"
#include "ne_system/Plugin.hpp"
#include "Renderer.hpp"

class RenderHypPlugin final : public Plugin {
public:
	void Build(App &app) override {
		app
			.InsertResource<RendererHyp>()
			.AddSystems(ScheduleLabel::STARTUP, SystemSet(Setup))
			.AddSystems(ScheduleLabel::RENDER, SystemSet(Render));
	}

	static void Setup(Resource<RendererHyp> renderer);
	static void Render(
		entt::registry& registry,
		Resource<RendererHyp> renderer,
		Resource<Camera> camera,
		Resource<Window> window
	);
};



#endif //RENDERHYPPLUGIN_HPP
