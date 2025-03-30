#ifndef SYSTEMSCHEDULE_HPP
#define SYSTEMSCHEDULE_HPP


#include <functional>

#include "ne_util/DirectedGraph.hpp"
#include "SystemSet.hpp"


class SystemSchedule {
public:
	using SystemId = DirectedGraph<SystemBase*>::NodeId;

	SystemSchedule() = default;
	explicit SystemSchedule(SystemSet&& root_set);

	[[nodiscard]] const std::vector<const SystemBase*>& GetCachedSchedule() const { return _cached_topsort; }

private:
	// DirectedGraph<System> _schedule_graph;
	SystemSet _root_set;
	std::vector<const SystemBase*> _cached_topsort;

	void Build();
};



#endif //SYSTEMSCHEDULE_HPP
