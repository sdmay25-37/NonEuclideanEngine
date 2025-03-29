#ifndef SYSTEMSET_HPP
#define SYSTEMSET_HPP


#include <functional>
#include <vector>

#include "ne_util/DirectedGraph.hpp"


// class System {
// public:
// 	System() = default;
//
// 	// Delete copy constructor and copy assignment
// 	System(const System& s) = delete;
// 	System& operator=(const System& s) = delete;
//
// 	// Default move constructor and move assignment
// 	System(System&&) noexcept = default;
// 	System& operator=(System&&) noexcept = default;
//
// private:
// 	std::function<void()> func;
// };

class SystemSet {
public:
	using RunCondition = std::function<bool()>;
	using System = std::function<void()>;

	template<typename... Systems>
	SystemSet(Systems&&... systems) {
		static_assert((std::is_same_v<Systems, System> && ...));
		(AddSystem(std::forward<Systems>(systems)), ...);
	}

	SystemSet& AddSystem(System&& system);

	SystemSet& Before(System&& system);
	SystemSet& After(System&& system);

	// TODO: implement ordering of system sets, which will require a dynamic 'Runnable' type for both Systems and SystemSets
	// SystemSet& Before(SystemSet&& system);
	// SystemSet& After(SystemSet&& system);

	SystemSet& RunIf(RunCondition&& rc);

	[[nodiscard]] const DirectedGraph<System>& GetSystemGraph() const { return _system_graph; }

private:
	using SystemId = DirectedGraph<System>::NodeId;
	DirectedGraph<System> _system_graph;
	std::vector<SystemId> _system_ids;
};



#endif //SYSTEMSET_HPP
