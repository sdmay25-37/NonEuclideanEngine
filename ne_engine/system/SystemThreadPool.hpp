#ifndef SYSTEMTHREADPOOL_HPP
#define SYSTEMTHREADPOOL_HPP

#include <functional>
#include <future>
#include <queue>
#include <unordered_set>


template<typename NodeType>
class SystemDAG {
public:
	using NodeId = std::size_t;

	NodeId AddNode(NodeType&& n);
	void AddEdge(NodeId u, NodeId v);

	std::vector<std::size_t> GetInDegrees() const { return std::vector(_in_degrees); }
	const std::vector<NodeId>& GetNodeNeighbors(NodeId u) const;
	const NodeType& GetNodeValue(NodeId u) const;

private:
	struct EdgeHash {
		std::size_t operator()(const std::pair<NodeId, NodeId>& e) const {
			return std::hash<std::size_t>()(e.first) ^ (std::hash<std::size_t>()(e.second) << 1);
		}
	};

	std::vector<NodeType> _nodes;
	std::vector<std::vector<NodeId>> _adjacency_lists;
	std::unordered_set<std::pair<NodeId, NodeId>, EdgeHash> _edge_set;
	std::vector<std::size_t> _in_degrees;
};


class SystemThreadPool {
	using SystemId = std::size_t;
	using SystemType = std::function<void()>;

public:
	explicit SystemThreadPool(size_t threads);
	~SystemThreadPool();

	// Execute the system plan defined in a DAG
	void execute(const SystemDAG<SystemType>& dag);

private:
	// List of active threads
	std::vector<std::thread> _threads;

	// Queue of systems ready to be run
	std::queue<std::pair<SystemId, const SystemType&>> _system_queue;

	// Stores the number of dependencies still needed before this system can be run
	std::vector<std::size_t> _system_dependency_counts;

	// Immutable reference to current DAG being executed
	const SystemDAG<SystemType>* _dag;

	// Number of systems waiting to be run this pass
	std::size_t _remaining;

	// Mult-threading control
	std::mutex _mtx;
	std::condition_variable _queue_cv;
	std::condition_variable _remaining_cv;
	bool _stop;

	// Method to add a system to a task queue
	void enqueue(SystemId id, const SystemType& func);
};



#endif //SYSTEMTHREADPOOL_HPP
