#include "SystemThreadPool.hpp"

#include <assert.h>

template<typename NodeType>
typename DAG<NodeType>::NodeId DAG<NodeType>::AddNode(NodeType&& n) {
	NodeId id = _nodes.size();

	_nodes.emplace_back(n);
	_adjacency_lists.push_back(std::vector<NodeId>());
	_in_degrees.push_back(0);

	return id;
}

template<typename NodeType>
void DAG<NodeType>::AddEdge(NodeId u, NodeId v) {
	// Check for existing edge
	assert(!_edge_set.contains({u, v}));

	// Check if graph contains U and V
	assert(u < _adjacency_lists.size() && v < _adjacency_lists.size());

	_adjacency_lists[u].push_back(v);
	_edge_set.insert({u, v});
	_in_degrees[v]++;
}

template<typename NodeType>
const std::vector<typename DAG<NodeType>::NodeId> & DAG<NodeType>::GetNodeNeighbors(NodeId u) const {
	// Check if graph contains U
	assert(u < _adjacency_lists.size());

	return _adjacency_lists[u];
}

template<typename NodeType>
const NodeType& DAG<NodeType>::GetNodeValue(NodeId u) const {
	// Check if graph contains U
	assert(u < _nodes.size());
	return _nodes[u];
}

template<typename NodeType>
const NodeType * DAG<NodeType>::GetNodeValuePtr(NodeId u) const {
	// Check if graph contains U
	assert(u < _nodes.size());
	return &_nodes[u];
}


SystemThreadPool::SystemThreadPool(size_t threads)
	: _dag(nullptr), _remaining(0), _stop(false) {

	for(size_t i = 0; i < threads; i++) {
		_threads.emplace_back([this] {
			while(true) {
				SystemId id;
				const std::function<void()>* task;

				{
					// Wait for available system or stop signal
					std::unique_lock lock(_mtx);
					_queue_cv.wait(lock, [this] { return _stop || !_system_queue.empty(); });

					// End thread if pool stopped and tasks empty
					if(_stop && _system_queue.empty()) return;

					// Take next system
					id = _system_queue.front().first;
					task = _system_queue.front().second;
					_system_queue.pop();
				}

				// Run task
				(*task)();

				{
					std::unique_lock lock(_mtx);

					_remaining--;
					_remaining_cv.notify_all();

					// Update system dependencies
					for(auto neighbor : _dag->GetNodeNeighbors(id)) {
						_system_dependency_counts[neighbor]--;

						// If neighbor has no more dependencies add it to the queue
						if(_system_dependency_counts[neighbor] == 0) {
							enqueue(neighbor, _dag->GetNodeValuePtr(neighbor));
						}
					}
				}
			}
		});
	}
}

SystemThreadPool::~SystemThreadPool() {
	{
		std::unique_lock lock(_mtx);
		_stop = true;
		_queue_cv.notify_all();
	}

	for(std::thread& thread : _threads) {
		thread.join();
	}
}

void SystemThreadPool::execute(const DAG<SystemType>& dag) {
	_dag = &dag;
	_system_dependency_counts = dag.GetInDegrees();

	std::unique_lock lock(_mtx);
	_remaining = _system_dependency_counts.size();

	for(std::size_t i = 0; i < _system_dependency_counts.size(); i++) {
		if(_system_dependency_counts[i] == 0) {
			enqueue(i, _dag->GetNodeValuePtr(i));
		}
	}

	// Wait until all systems have finished
	_remaining_cv.wait(lock, [this] { return _remaining == 0; });
}


void SystemThreadPool::enqueue(SystemId id, const SystemType* func) {
	std::unique_lock lock(_mtx);
	_system_queue.emplace(id, func);
	_queue_cv.notify_one();
}
