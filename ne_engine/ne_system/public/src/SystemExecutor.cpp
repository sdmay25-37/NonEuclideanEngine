#include "ne_system/SystemExecutor.hpp"


//-- SINGLE THREADED EXECUTOR --//

class SingleThreadedExecutor final : public SystemExecutor {
public:
	explicit SingleThreadedExecutor(entt::registry& registry, ResourceManager& resource_manager)
		: SystemExecutor(registry, resource_manager) {}
	void Execute(SystemSchedule& schedule) const override;
};

void SingleThreadedExecutor::Execute(SystemSchedule& schedule) const {
	auto schedule_list = schedule.GetCachedSchedule();
	for(const auto system : schedule_list) {
		system->Invoke(_registry, _resource_manager);
	}
}


//-- MULTI THREADED EXECUTOR --//

class MultiThreadedExecutor final : public SystemExecutor {
public:
	explicit MultiThreadedExecutor(entt::registry& registry, ResourceManager& resource_manager)
		: SystemExecutor(registry, resource_manager) {}
	void Execute(SystemSchedule& schedule) const override;
};

void MultiThreadedExecutor::Execute(SystemSchedule& schedule) const {
	// TODO
}


//-- SYSTEM EXECUTOR --//

std::unique_ptr<SystemExecutor> SystemExecutor::Create(const Type type, entt::registry& registry, ResourceManager& resource_manager) {
	switch(type) {
		case Type::SingleThreaded:
			return std::make_unique<SingleThreadedExecutor>(registry, resource_manager);
		case Type::MultiThreaded:
			return std::make_unique<MultiThreadedExecutor>(registry, resource_manager);
	}

	return nullptr;
}

