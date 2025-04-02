#ifndef DIRECTEDGRAPH_HPP
#define DIRECTEDGRAPH_HPP


#include <assert.h>
#include <memory>
#include <unordered_set>
#include <vector>


template<typename NodeType>
class DirectedGraph {
public:
	using NodeId = std::size_t;

	NodeId AddNode(NodeType&& n);
	void AddEdge(NodeId u, NodeId v);

	[[nodiscard]] std::vector<std::size_t> GetInDegrees() const { return std::vector(_in_degrees); }
	[[nodiscard]] const std::vector<NodeId>& GetNodeNeighbors(NodeId u) const;
	[[nodiscard]] const NodeType& GetNodeValue(NodeId u) const;
	[[nodiscard]] const NodeType* GetNodeValuePtr(NodeId u) const;

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

template<typename NodeType>
typename DirectedGraph<NodeType>::NodeId DirectedGraph<NodeType>::AddNode(NodeType&& n) {
	NodeId id = _nodes.size();

	_nodes.emplace_back(std::move(n));
	_adjacency_lists.emplace_back();
	_in_degrees.push_back(0);

	return id;
}

template<typename NodeType>
void DirectedGraph<NodeType>::AddEdge(NodeId u, NodeId v) {
	// Check for existing edge
	assert(_edge_set.find({u, v}) == _edge_set.end());

	// Check if graph contains U and V
	assert(u < _adjacency_lists.size() && v < _adjacency_lists.size());

	_adjacency_lists[u].push_back(v);
	_edge_set.insert({u, v});
	_in_degrees[v]++;
}

template<typename NodeType>
const std::vector<typename DirectedGraph<NodeType>::NodeId> & DirectedGraph<NodeType>::GetNodeNeighbors(NodeId u) const {
	// Check if graph contains U
	assert(u < _adjacency_lists.size());

	return _adjacency_lists[u];
}

template<typename NodeType>
const NodeType& DirectedGraph<NodeType>::GetNodeValue(NodeId u) const {
	// Check if graph contains U
	assert(u < _nodes.size());
	return _nodes[u];
}

template<typename NodeType>
const NodeType* DirectedGraph<NodeType>::GetNodeValuePtr(NodeId u) const {
	// Check if graph contains U
	assert(u < _nodes.size());
	return &_nodes[u];
}

#endif //DIRECTEDGRAPH_HPP
