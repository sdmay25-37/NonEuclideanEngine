
#include "ne_system/SystemSet.hpp"

SystemSet& SystemSet::AddSystem(System&& system) {
	_system_ids.push_back(
		_dependency_graph.AddNode(std::move(system))
	);
	return *this;
}

SystemSet& SystemSet::Before(System&& system) {
	SystemId id = _dependency_graph.AddNode(std::move(system));

	// Create dependency edges from current systems to this system
	for(SystemId sys_id : _system_ids) {
		_dependency_graph.AddEdge(sys_id, id);
	}

	_system_ids.push_back(id);
	return *this;
}

SystemSet& SystemSet::After(System&& system) {
	SystemId id = _dependency_graph.AddNode(std::move(system));

	// Create dependency edges from this system to current systems
	for(SystemId sys_id : _system_ids) {
		_dependency_graph.AddEdge(id, sys_id);
	}

	_system_ids.push_back(id);
	return *this;
}

