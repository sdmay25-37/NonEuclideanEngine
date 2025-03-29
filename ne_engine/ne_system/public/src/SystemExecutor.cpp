#include "ne_system/SystemExecutor.hpp"


//-- SINGLE THREADED EXECUTOR --//

class SingleThreadedExecutor final : public SystemExecutor {
public:
	explicit SingleThreadedExecutor(entt::registry& registry) : SystemExecutor(registry) {}
	void Execute(SystemSchedule& schedule) const override;
};

void SingleThreadedExecutor::Execute(SystemSchedule& schedule) const {
	auto schedule_list = schedule.GetCachedSchedule();
	for(const auto system : schedule_list) {
		(*system)(_registry);
	}
}


//-- MULTI THREADED EXECUTOR --//

class MultiThreadedExecutor final : public SystemExecutor {
public:
	explicit MultiThreadedExecutor(entt::registry& registry) : SystemExecutor(registry) {}
	void Execute(SystemSchedule& schedule) const override;
};

void MultiThreadedExecutor::Execute(SystemSchedule& schedule) const {
	// TODO
}


//-- SYSTEM EXECUTOR --//

std::unique_ptr<SystemExecutor> SystemExecutor::Create(const Type type, entt::registry& registry) {
	switch(type) {
		case Type::SingleThreaded:
			return std::make_unique<SingleThreadedExecutor>(registry);
		case Type::MultiThreaded:
			return std::make_unique<MultiThreadedExecutor>(registry);
	}

	return nullptr;
}

