#include "SystemThreadPool.hpp"

#include <iostream>


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
							Enqueue(neighbor, _dag->GetNodeValuePtr(neighbor));
							_queue_cv.notify_one();
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

void SystemThreadPool::Execute(const DAG<SystemType>& dag) {
	_dag = &dag;
	_system_dependency_counts = dag.GetInDegrees();

	std::unique_lock lock(_mtx);
	_remaining = _system_dependency_counts.size();

	for(std::size_t i = 0; i < _system_dependency_counts.size(); i++) {
		if(_system_dependency_counts[i] == 0) {
			Enqueue(i, _dag->GetNodeValuePtr(i));
			_queue_cv.notify_one();
		}
	}

	// Wait until all systems have finished
	_remaining_cv.wait(lock, [this] { return _remaining == 0; });
}


void SystemThreadPool::Enqueue(SystemId id, const SystemType* func) {
	_system_queue.emplace(id, func);
	_queue_cv.notify_one();
}
