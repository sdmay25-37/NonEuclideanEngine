#ifndef DEFAULTPLUGINS_HPP
#define DEFAULTPLUGINS_HPP

#include "ne_system/Plugin.hpp"
#include "RenderPlugin.hpp"
#include "ne_plugin/input/InputPlugin.hpp"

class DefaultPlugins final : public Plugin
{
public:
	void Build(App &app) override
	{
		app
			.AddPlugin<InputPlugin>()
			.AddPlugin<RenderPlugin>();
	}
};

#endif // DEFAULTPLUGINS_HPP
