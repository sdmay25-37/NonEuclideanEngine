#include <ctime>
#include <iostream>
#include <thread>

#include "App.hpp"
#include "Renderer.hpp"
#include "plugin/window/Window.hpp"


constexpr int UPDATES_PER_SECOND = 60;
constexpr double SECONDS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
constexpr int MAX_FRAMESKIP = 5;


void App::Run() {

	// TODO: remove all references of window as it is now an optional plugin
	Startup();

	auto window = _resource_manager.get<Window>();

	while(!glfwWindowShouldClose(window->ptr)) {
		Update();
		Render();
	}

	Cleanup();
}

void App::Startup() {
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
}

