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

    static std::unique_ptr<SystemExecutor> Create(Type type);
	virtual void Execute(SystemSchedule& schedule) const = 0;
	virtual ~SystemExecutor() = default;

private:


};



#endif //SYSTEMEXECUTOR_HPP
