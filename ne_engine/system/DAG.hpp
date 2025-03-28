#ifndef DAG_HPP
#define DAG_HPP


#include <unordered_set>
#include <vector>


template<typename NodeType>
class DAG {
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


#endif //DAG_HPP
