#pragma once
#include "graph_concepts.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace graph::algorithms::floyd_warshall {

template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  Id parent;

  Node();
};

template <concepts::Numeric WeightType>
using Map = std::unordered_map<Id, std::unordered_map<Id, Node<WeightType>>>;

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Map<WeightType> visit(G &graph, C &&weights);

} // namespace graph::algorithms::floyd_warshall

#include "algorithms/floyd_warshall.i.hpp"