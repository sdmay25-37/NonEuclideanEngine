#ifndef SYSTEMSET_HPP
#define SYSTEMSET_HPP


#include <functional>
#include <vector>

#include "ne_util/DirectedGraph.hpp"


using System = std::function<void()>;

class SystemSet {
public:
	using RunCondition = std::function<bool()>;

	template<typename... Systems>
	explicit SystemSet(Systems&&... systems) {
		static_assert((std::is_convertible_v<Systems, System> && ...));
		(AddSystem(std::forward<Systems>(systems)), ...);
	}

	SystemSet& AddSystem(System&& system);

	SystemSet& Before(System&& system);
	SystemSet& After(System&& system);

	// TODO: implement ordering of system sets, which will require a dynamic 'Runnable' type for both Systems and SystemSets
	// SystemSet& Before(SystemSet&& system);
	// SystemSet& After(SystemSet&& system);

	SystemSet& RunIf(RunCondition&& rc);

	[[nodiscard]] const DirectedGraph<System>& GetDependencyGraph() const { return _dependency_graph; }

private:
	using SystemId = DirectedGraph<System*>::NodeId;
	DirectedGraph<System> _dependency_graph;
	std::vector<SystemId> _system_ids;
};



#endif //SYSTEMSET_HPP
