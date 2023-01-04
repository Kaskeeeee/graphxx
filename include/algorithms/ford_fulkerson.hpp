#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace graph::algorithms::ford_fulkerson {

template <concepts::Numeric WeightType> struct EdgeFlow {
  Id edge_id;
  Id return_edge_id;
  WeightType capacity;
  WeightType flow;

};

template <concepts::Numeric WeightType>
using Tree = std::unordered_map<Id, Node<WeightType>>;

/// @brief Implementation of ford_fulkerson algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param sink sink vertex
/// @param edges_capacity edges capacity
/// @return maximum flow from source to sink in the given graph
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
WeightType visit(const G &graph, const Vertex &source, const Vertex &sink, C &&edges_capacity);

} // namespace graph::algorithms::ford_fulkerson

#include "algorithms/ford_fulkerson.i.hpp"