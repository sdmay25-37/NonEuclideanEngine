#ifndef PLUGIN_HPP
#define PLUGIN_HPP

class App;

class Plugin {
public:
	virtual void Build(App& app) = 0;
	virtual ~Plugin() = default;
};

enum class ScheduleLabel {
	PRE_STARTUP, STARTUP,
	UPDATE,
	RENDER, POST_RENDER,
	CLEANUP,
	MAX_VALUE
};



#endif //PLUGIN_HPP
