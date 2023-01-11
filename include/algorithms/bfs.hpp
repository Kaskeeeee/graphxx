#pragma once

#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <unordered_map>

namespace graph::algorithms::bfs {

/// @brief Structure to store information about graph vertices during the visit
struct BFSVertex {
  /// @brief Vertex visitation status
  VertexStatus status;
  /// @brief Number of edges in the shortest path from the source vertex
  int distance = -1;
  /// @brief Id of the predecessor Vertex in BFS Tree
  Id parent = -1;
};

/// @brief flatten Tree that collects all BFSVertex structs generated during the
///        visit
using BFSTree = std::unordered_map<Id, BFSVertex>;

/// @brief Performs a breadth-first traversal of a graph. A breadth-first
///        traversal visits vertices that are closer to the source before
///        visiting vertices that are further away.
/// @tparam G type of input graph
/// @param graph input graph
/// @param source source vertex
/// @return flatten tree as described for type BFSTree
template <concepts::Graph G> BFSTree visit(const G &graph, Vertex source);

/// @brief Performs a breadth-first traversal of a graph. A breadth-first
///        traversal visits vertices that are closer to the source before
///        visiting vertices that are further away. For each visited node
///        function `callback` is called.
/// @tparam G type of input graph
/// @param graph input graph
/// @param source source vertex
/// @param callback function to call when a new node is visited
/// @return flatten tree as described for type BFSTree
template <concepts::Graph G>
BFSTree visit(const G &graph, Vertex source,
              const std::function<void(Vertex)> &callback);

} // namespace graph::algorithms::bfs

#include "algorithms/bfs.i.hpp"