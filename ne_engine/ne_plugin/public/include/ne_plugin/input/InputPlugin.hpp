#ifndef INPUTPLUGIN_HPP
#define INPUTPLUGIN_HPP

#include "ne_system/Plugin.hpp"

class InputPlugin final : public Plugin
{
public:
	void Build(App &app) override
	{
		app.AddSystems(ScheduleLabel::UPDATE, SystemSet(WindowUpdate));
	}

private:
	static void WindowUpdate(Resource<Window> window, Resource<AppClose> app_close)
	{
		window->PollEvents();
		app_close->value = window->ShouldClose();
	}
};

#endif // INPUTPLUGIN_HPP
