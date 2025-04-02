#ifndef SYSTEMEXECUTOR_HPP
#define SYSTEMEXECUTOR_HPP


#include <memory>

#include "Resource.hpp"
#include "SystemSchedule.hpp"


class SystemExecutor {
public:

	enum class Type {
    	SingleThreaded,
   		MultiThreaded
    };

    static std::unique_ptr<SystemExecutor> Create(Type type, entt::registry& registry, ResourceManager& resource_manager);
	virtual void Execute(SystemSchedule& schedule) const = 0;
	virtual ~SystemExecutor() = default;

protected:
	entt::registry& _registry;
	ResourceManager& _resource_manager;

	explicit SystemExecutor(entt::registry& registry, ResourceManager& resource_manager)
		: _registry(registry), _resource_manager(resource_manager) {};
};



#endif //SYSTEMEXECUTOR_HPP
