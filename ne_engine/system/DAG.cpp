#include "DAG.hpp"

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