#include "App.hpp"
#include "Renderer.hpp"

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
	_executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded, _registry, _resource_manager);

	_executor->Execute(_schedules[ScheduleLabel::PRE_STARTUP]);
	_executor->Execute(_schedules[ScheduleLabel::STARTUP]);
}

void App::Update() {
	_executor->Execute(_schedules[ScheduleLabel::UPDATE]);

	// TODO: remove all references of window as it is now an optional plugin
	// auto window = _resource_manager.Get<Window>();
	// if(window->ShouldClose()) {
	// 	_resource_manager.Insert<AppClose>(true);
	// }
}

void App::Render() {
	_executor->Execute(_schedules[ScheduleLabel::RENDER]);
	_executor->Execute(_schedules[ScheduleLabel::POST_RENDER]);
}

void App::Cleanup() {
	_executor->Execute(_schedules[ScheduleLabel::CLEANUP]);
}

