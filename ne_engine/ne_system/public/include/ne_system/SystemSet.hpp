#ifndef SYSTEMSET_HPP
#define SYSTEMSET_HPP


#include <functional>
#include <vector>
#include <entt/entt.hpp>

#include "ne_util/DirectedGraph.hpp"
#include "System.hpp"

class SystemSet {
public:
	using RunCondition = std::function<bool()>;

	SystemSet() = default;

	template<typename... Systems>
	explicit SystemSet(Systems&&... systems) {
		(AddSystem(std::forward<Systems>(systems)), ...);
	}

	template<typename F>
	SystemSet& AddSystem(F&& system) {
		_system_ids.push_back(
			_dependency_graph.AddNode(std::move(CreateSystem(system)))
		);
		return *this;
	}

	template<typename F>
	SystemSet& Before(F&& system) {
		SystemId id = _dependency_graph.AddNode(std::move(CreateSystem(system)));

		// Create dependency edges from current systems to this system
		for(SystemId sys_id : _system_ids) {
			_dependency_graph.AddEdge(sys_id, id);
		}

		_system_ids.push_back(id);
		return *this;
	}

	template<typename F>
	SystemSet& After(F&& system) {
		SystemId id = _dependency_graph.AddNode(std::move(CreateSystem(system)));

		// Create dependency edges from this system to current systems
		for(SystemId sys_id : _system_ids) {
			_dependency_graph.AddEdge(id, sys_id);
		}

		_system_ids.push_back(id);
		return *this;
	}

	// TODO: implement ordering of system sets, which will require a dynamic 'Runnable' type for both Systems and SystemSets
	// SystemSet& Before(SystemSet&& system);
	// SystemSet& After(SystemSet&& system);

	SystemSet& RunIf(RunCondition&& rc);

	[[nodiscard]] const DirectedGraph<std::unique_ptr<SystemBase>>& GetDependencyGraph() const { return _dependency_graph; }

private:
	using SystemId = DirectedGraph<std::unique_ptr<SystemBase>>::NodeId;
	DirectedGraph<std::unique_ptr<SystemBase>> _dependency_graph;
	std::vector<SystemId> _system_ids;
};







#endif //SYSTEMSET_HPP
