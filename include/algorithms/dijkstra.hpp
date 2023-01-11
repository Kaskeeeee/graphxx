#pragma once

#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graph::algorithms::dijkstra {

/// @brief Node containing informations about the parent (which is the previous
/// node)
///        on the shortest path and its distance from the source
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  Id parent;
};

/// @brief flatten Tree that collects all Node structs containing information
///        based on the source vertex
///        i.e. for each vertex its distance from the source and the previous
///        node in the shortest path
/// @tparam WeightType
template <concepts::Numeric WeightType>
using Tree = std::unordered_map<Id, Node<WeightType>>;

/// @brief Implementation of dijkstra algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param weights edges weights
/// @return flatten tree as described for type Tree<WeightType>
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights);

} // namespace graph::algorithms::dijkstra

#include "algorithms/dijkstra.i.hpp"