#ifndef WINDOWPLUGIN_HPP
#define WINDOWPLUGIN_HPP

#include "App.hpp"
#include "GLFWWindow.hpp"
#include "Window.hpp"
#include "ne_system/Plugin.hpp"
#include "ne_system/Resource.hpp"

class WindowPlugin final : public Plugin {
public:

	void Build(App& app) override {

		app
			.InsertResourceBase<Window, GLFWWindow>(800, 600)
			.AddSystems(ScheduleLabel::UPDATE, SystemSet(WindowUpdate))
			.AddSystems(ScheduleLabel::POST_RENDER, SystemSet(WindowPostRender));
	}

private:

	static void WindowUpdate();
	static void WindowPostRender(Resource<Window> window);

};


#endif //WINDOWPLUGIN_HPP
