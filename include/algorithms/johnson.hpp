#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graph::algorithms::johnson {

/// @brief Node containing informations about its distance and
///        the previous node in the shortest path from a given source
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  Id parent;
};

/// @brief A simple map of maps of ids to Nodes
/// @tparam WeightType
template <concepts::Numeric WeightType>
using Map = std::unordered_map<Id, std::unordered_map<Id, Node<WeightType>>>;

/// @brief Implementation of johnson algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
///         edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param weights edges weights
/// @return a map of maps containing all shortest paths
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Map<WeightType> visit(G &graph, C &&weights);

} // namespace graph::algorithms::johnson

#include "algorithms/johnson.i.hpp"