#ifndef PLUGIN_HPP
#define PLUGIN_HPP

class App;

class Plugin {
public:
	virtual void build(App& app) = 0;
	virtual ~Plugin() = default;
};



#endif //PLUGIN_HPP
