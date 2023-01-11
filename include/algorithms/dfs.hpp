#pragma once

#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <unordered_map>

namespace graph::algorithms::dfs {

/// @brief Structure to store information about graph vertices during the visit
struct DFSVertex {
  /// @brief Vertex visitation status
  VertexStatus status;
  /// @brief Id of the predecessor Vertex in DFS Tree
  Id parent = -1;
  /// @brief Counter indicating when vertex is discovered
  int discovery_time = -1;
  /// @brief Counter indicating when the processing of vertex is finished
  int finishing_time = -1;
};

/// @brief flatten Tree that collects all DFSVertex structs generated during the
///        visit
using DFSTree = std::unordered_map<Id, DFSVertex>;

/// @brief Performs a depth-first traversal of the graph. A depth-first
///        traversal chooses a vertex adjacent to the current vertex to visit
///        next. If all adjacent vertices have already been discovered, or there
///        are no adjacent vertices, then the algorithm backtracks to the last
///        vertex that had undiscovered neighbors.
/// @tparam G type of input graph
/// @param graph input graph
/// @param source vertex
/// @return flatten tree as described for type DFSTree
template <concepts::Graph G> DFSTree visit(const G &graph, Vertex source);

/// @brief Performs a depth-first traversal of the graph. A depth-first
///        traversal chooses a vertex adjacent to the current vertex to visit
///        next. If all adjacent vertices have already been discovered, or there
///        are no adjacent vertices, then the algorithm backtracks to the last
///        vertex that had undiscovered neighbors. For each visited node
///        function `callback` is called.
/// @tparam G type of input graph
/// @param graph input graph
/// @param source vertex
/// @param callback function to call when a new node is visited
/// @return flatten tree as described for type DFSTree
template <concepts::Graph G>
DFSTree visit(const G &graph, Vertex source,
              const std::function<void(Vertex)> &callback);

} // namespace graph::algorithms::dfs

#include "algorithms/dfs.i.hpp"