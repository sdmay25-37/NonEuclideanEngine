#ifndef SYSTEMEXECUTOR_HPP
#define SYSTEMEXECUTOR_HPP


#include <memory>

#include "SystemSchedule.hpp"


class SystemExecutor {
public:

	enum class Type {
    	SingleThreaded,
   		MultiThreaded
    };

    static std::unique_ptr<SystemExecutor> Create(Type type, entt::registry& registry);
	virtual void Execute(SystemSchedule& schedule) const = 0;
	virtual ~SystemExecutor() = default;

protected:
	entt::registry& _registry;

	explicit SystemExecutor(entt::registry& registry) : _registry(registry) {};
};



#endif //SYSTEMEXECUTOR_HPP
