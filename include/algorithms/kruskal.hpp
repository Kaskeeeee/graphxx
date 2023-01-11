#pragma once

#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <vector>

namespace graph::algorithms::kruskal {

/// @brief flatten Tree that will collect the Edge ids of the minimum spanning
/// tree
using Tree = std::vector<Id>;

/// @brief Map that divides the vertices into "clusters" and allows us to check
/// if two vertices belong to the same cluster or not and hence decide whether
/// adding an edge creates a cycle
using ClusterMap = std::unordered_map<Id, int>;

/// @brief Implementation of kruskal algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param weights edges weights
/// @return flatten tree as described for type Tree
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree visit(const G &graph, C &&weights);

} // namespace graph::algorithms::kruskal

#include "algorithms/kruskal.i.hpp"