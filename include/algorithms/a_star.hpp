#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace graph::algorithms::a_star {

template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  WeightType heuristic_distance;
  Id parent;
};

template <concepts::Numeric WeightType>
using Tree = std::unordered_map<Id, Node<WeightType>>;

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &v, C &&edges_weights,
                       C &&heuristic_weights);

} // namespace graph::algorithms::a_star

#include "algorithms/a_star.i.hpp"