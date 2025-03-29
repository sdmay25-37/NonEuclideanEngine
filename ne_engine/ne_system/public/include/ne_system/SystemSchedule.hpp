#ifndef SYSTEMSCHEDULE_HPP
#define SYSTEMSCHEDULE_HPP


#include <functional>

#include "ne_system/DAG.hpp"


class SystemSchedule {
public:
	using System = std::function<void()>;
	using SystemId = DAG<System>::NodeId;

	explicit SystemSchedule(DAG<System>&& schedule_graph);

	void Execute() const;

private:
	DAG<System> _schedule_graph;
	std::vector<const System*> _cached_topsort;

	void Build();
};



#endif //SYSTEMSCHEDULE_HPP
