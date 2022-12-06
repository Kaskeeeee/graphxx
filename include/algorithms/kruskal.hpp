#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <vector>

namespace graph::algorithms::kruskal {

// Vector of edge Ids
using Tree = std::vector<Id>;

// Map that divides the vertices into "clusters" and allows us to check if two
// vertices belong to the same cluster or not and hence decide whether adding
// an edge creates a cycle
using ClusterMap = std::unordered_map<Id, int>;

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree visit(const G &graph, C &&weights);

} // namespace graph::algorithms::kruskal

#include "algorithms/kruskal.i.hpp"