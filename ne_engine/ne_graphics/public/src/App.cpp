#include "App.hpp"

#include <iostream>

#include "Renderer.hpp"
#include "GLFW/glfw3.h"

void App::Run() {

	Startup();

	_resource_manager.Insert<AppClose>(false);

	while(!_resource_manager.Get<AppClose>()->value) {
		Update();
		Render();
	}

	Cleanup();
}

void App::Startup() {
	std::cout << "NE_ENGINE::STARTUP" << std::endl;

	_executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded, _registry, _resource_manager);

	_executor->Execute(_schedules[ScheduleLabel::PRE_STARTUP]);
	_executor->Execute(_schedules[ScheduleLabel::STARTUP]);
}

void App::Update() {
	_executor->Execute(_schedules[ScheduleLabel::UPDATE]);
}

void App::Render() {
	_executor->Execute(_schedules[ScheduleLabel::RENDER]);
	_executor->Execute(_schedules[ScheduleLabel::POST_RENDER]);
}

void App::Cleanup() {
	_executor->Execute(_schedules[ScheduleLabel::CLEANUP]);

	_resource_manager.~ResourceManager();

	glfwTerminate();
}

