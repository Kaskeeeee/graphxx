#pragma once

#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graph::algorithms::a_star {

/// @brief Node containing informations about its distance,
///        its heurisitc_distance (initially given by the user) and
///        the previous node in the shortest path from a given source
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  WeightType heuristic_distance;
  Id parent;
};

/// @brief flatten Tree that collects all Node structs containing information
///        based on the source vertex
///        i.e. for each vertex its distance and the heuristic one from the
///        source and the previous node in the shortest path
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType>
using Tree = std::unordered_map<Id, Node<WeightType>>;

/// @brief Implementation of a_star algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param edges_weights edges weights
/// @param heuristic_weights heuristic distances for each vertex
/// @return flatten tree as described for type Tree<WeightType>
template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&edges_weights,
                       C &&heuristic_weights);

} // namespace graph::algorithms::a_star

#include "algorithms/a_star.i.hpp"