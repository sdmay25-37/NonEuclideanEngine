#ifndef SYSTEMSCHEDULE_HPP
#define SYSTEMSCHEDULE_HPP


#include <functional>

#include "ne_util/DirectedGraph.hpp"


class SystemSchedule {
public:
	using System = std::function<void()>;
	using SystemId = DirectedGraph<System>::NodeId;

	explicit SystemSchedule(DirectedGraph<System>&& schedule_graph);

	void Execute() const;

private:
	DirectedGraph<System> _schedule_graph;
	std::vector<const System*> _cached_topsort;

	void Build();
};



#endif //SYSTEMSCHEDULE_HPP
