#pragma once
#include "graph_concepts.hpp"
#include "utils.hpp"
#include <unordered_map>

namespace graph::algorithms::floyd_warshall {

/// @brief Node containing informations about its distance and
///        and the previous node on the shortest path from a given source
/// @tparam WeightType
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  Id parent;

  Node();
};

/// @brief A simple map of maps of ids to Nodes
/// @tparam WeightType
template <concepts::Numeric WeightType>
using Map = std::unordered_map<Id, std::unordered_map<Id, Node<WeightType>>>;

/// @brief Implementantation of Floyd Warhsall algorithm for multi source
/// shortest paths
/// @tparam G graph type
/// @tparam C edge weights supplier (overloads operator[])
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param weights edges weights
/// @return a map of maps containing all shortest paths
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Map<WeightType> visit(G &graph, C &&weights);

} // namespace graph::algorithms::floyd_warshall

#include "algorithms/floyd_warshall.i.hpp"