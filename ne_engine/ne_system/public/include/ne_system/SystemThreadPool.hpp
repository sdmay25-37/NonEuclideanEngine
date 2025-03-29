#ifndef SYSTEMTHREADPOOL_HPP
#define SYSTEMTHREADPOOL_HPP


#include <functional>
#include <future>
#include <queue>

#include "ne_util/DirectedGraph.hpp"


class SystemThreadPool {
	using SystemId = std::size_t;
	using SystemType = std::function<void()>;

public:
	explicit SystemThreadPool(size_t threads);
	~SystemThreadPool();

	// Execute the system plan defined in a DAG
	void Execute(const DirectedGraph<SystemType>& dag);

private:
	// List of active threads
	std::vector<std::thread> _threads;

	// Queue of systems ready to be run
	std::queue<std::pair<SystemId, const SystemType*>> _system_queue;

	// Stores the number of dependencies still needed before this system can be run
	std::vector<std::size_t> _system_dependency_counts;

	// Immutable reference to current DAG being executed
	const DirectedGraph<SystemType>* _dag;

	// Number of systems waiting to be run this pass
	std::size_t _remaining;

	// Mult-threading control
	std::mutex _mtx;
	std::condition_variable _queue_cv;
	std::condition_variable _remaining_cv;
	bool _stop;

	// Method to add a system to a task queue
	void Enqueue(SystemId id, const SystemType* func);
};



#endif //SYSTEMTHREADPOOL_HPP
