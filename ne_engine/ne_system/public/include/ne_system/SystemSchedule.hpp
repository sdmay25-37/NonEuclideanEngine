#ifndef SYSTEMSCHEDULE_HPP
#define SYSTEMSCHEDULE_HPP


#include <functional>

#include "ne_util/DirectedGraph.hpp"
#include "SystemSet.hpp"


class SystemSchedule {
public:
	using System = std::function<void()>;
	using SystemId = DirectedGraph<System>::NodeId;

	explicit SystemSchedule(SystemSet&& root_set);

	// [[nodiscard]] const DirectedGraph<System>& GetScheduleGraph() const { return _schedule_graph; }

	void Execute() const;

private:
	// DirectedGraph<System> _schedule_graph;
	SystemSet _root_set;
	std::vector<const System*> _cached_topsort;

	void Build();
};



#endif //SYSTEMSCHEDULE_HPP
