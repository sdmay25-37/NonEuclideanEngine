#ifndef WINDOWPLUGIN_HPP
#define WINDOWPLUGIN_HPP

#include "App.hpp"
#include "Window.hpp"
#include "ne_system/Plugin.hpp"
#include "ne_system/Resource.hpp"

class WindowPlugin final : public Plugin {
public:

	void Build(App& app) override {

		app
			.InsertResource<Window>(800, 600)
			.AddSystems(ScheduleLabel::PRE_STARTUP, SystemSet(WindowSetup))
			.AddSystems(ScheduleLabel::UPDATE, SystemSet(WindowUpdate))
			.AddSystems(ScheduleLabel::POST_RENDER, SystemSet(WindowPostRender));
	}

private:

	static void WindowSetup(Resource<Window> window);
	static void WindowUpdate();
	static void WindowPostRender(Resource<Window> window);

};


#endif //WINDOWPLUGIN_HPP
