#include <queue>

#include "ne_system/SystemSchedule.hpp"


SystemSchedule::SystemSchedule(SystemSet&& root_set)
	: _root_set(std::move(root_set)) {
	Build();
}

void SystemSchedule::Build() {
	// Create a topological sort using Kahn's algorithm
	_cached_topsort.clear();
	const DirectedGraph<std::unique_ptr<SystemBase>>& schedule_graph = _root_set.GetDependencyGraph();

	std::vector<SystemId> in_degrees = schedule_graph.GetInDegrees();
	std::queue<SystemId> q;

	for(SystemId id = 0; id < in_degrees.size(); id++) {
		if(in_degrees[id] == 0) {
			q.push(id);
		}
	}

	while(!q.empty()) {
		SystemId id = q.front();
		q.pop();

		_cached_topsort.push_back(schedule_graph.GetNodeValuePtr(id)->get());

		for(SystemId neighbor : schedule_graph.GetNodeNeighbors(id)) {
			in_degrees[neighbor]--;
			if(in_degrees[neighbor] == 0) {
				q.push(neighbor);
			}
		}
	}
}