#ifndef WINDOWPLUGIN_HPP
#define WINDOWPLUGIN_HPP

#include <Window.hpp>
#include "ne_system/Plugin.hpp"
#include "ne_system/Resource.hpp"
#include "App.hpp"

class WindowPlugin final : public Plugin {
public:

	void Build(App& app) override {
		app
			.InsertResource<Window>()
			.AddSystems(ScheduleLabel::PRE_STARTUP, SystemSet(WindowSetup));
	}

	static void WindowSetup(Resource<Window> window);
};


#endif //WINDOWPLUGIN_HPP
