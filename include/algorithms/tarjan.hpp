#pragma once

#include "graph_concepts.hpp"

#include <unordered_map>
#include <vector>

namespace graph::algorithms::tarjan {

/// @brief Vertex containing informations for the algorithm
struct TarjanVertex {
  int index = -1;
  int low_link = -1;
  bool on_stack = false;
};

/// @brief a map of id to TarjanVertex
using TarjanTree = std::unordered_map<Id, TarjanVertex>;

/// @brief a vector of id
using StackVector = std::vector<Id>;

/// @brief a vector containing all the strongly connected components
using SCCVector = std::vector<StackVector>;

/// @brief Implementation of tarjan algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @return a vector containing all the strongly connected components
template <concepts::Graph G> SCCVector visit(const G &graph);

} // namespace graph::algorithms::tarjan

#include "algorithms/tarjan.i.hpp"