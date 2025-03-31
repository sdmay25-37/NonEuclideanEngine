#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include "ne_system/Plugin.hpp"

struct Window {
	GLFWwindow* ptr;
};

class WindowPlugin final : public Plugin {
public:

	void build(App& app) override {

    }
};



#endif //WINDOW_HPP
