#include "ne_system/SystemExecutor.hpp"


//-- SINGLE THREADED EXECUTOR --//

class SingleThreadedExecutor final : public SystemExecutor {
public:
	void Execute(SystemSchedule& schedule) const override;
};

void SingleThreadedExecutor::Execute(SystemSchedule& schedule) const {
	schedule.Execute();
}


//-- MULTI THREADED EXECUTOR --//

class MultiThreadedExecutor final : public SystemExecutor {
public:
	void Execute(SystemSchedule& schedule) const override;
};

void MultiThreadedExecutor::Execute(SystemSchedule& schedule) const {

}


//-- SYSTEM EXECUTOR --//

std::unique_ptr<SystemExecutor> SystemExecutor::Create(const Type type) {
	switch(type) {
		case Type::SingleThreaded:
			return std::make_unique<SingleThreadedExecutor>();
		case Type::MultiThreaded:
			return std::make_unique<MultiThreadedExecutor>();
	}

	return nullptr;
}

