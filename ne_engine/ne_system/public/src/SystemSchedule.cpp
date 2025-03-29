#include <queue>

#include "ne_system/SystemSchedule.hpp"


SystemSchedule::SystemSchedule(DAG<System>&& schedule_graph)
	: _schedule_graph(std::move(schedule_graph)) {
	Build();
}

void SystemSchedule::Execute() const {
	for(auto system : _cached_topsort) {
		(*system)();
	}
}

void SystemSchedule::Build() {
	// Create a topological sort using Kahn's algorithm
	_cached_topsort.clear();

	std::vector<SystemId> in_degrees = _schedule_graph.GetInDegrees();
	std::queue<SystemId> q;

	for(SystemId id = 0; id < in_degrees.size(); id++) {
		if(in_degrees[id] == 0) {
			q.push(id);
		}
	}

	while(!q.empty()) {
		SystemId id = q.front();
		q.pop();

		_cached_topsort.push_back(_schedule_graph.GetNodeValuePtr(id));

		for(SystemId neighbor : _schedule_graph.GetNodeNeighbors(id)) {
			in_degrees[neighbor]--;
			if(in_degrees[neighbor] == 0) {
				q.push(neighbor);
			}
		}
	}
}