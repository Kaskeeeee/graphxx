#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graph::algorithms::dijkstra {

template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  Id parent;
};

template <concepts::Numeric WeightType>
using Tree = std::unordered_map<Id, Node<WeightType>>;

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights);

} // namespace graph::algorithms::dijkstra

#include "algorithms/dijkstra.i.hpp"