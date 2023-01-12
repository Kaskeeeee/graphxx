/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
 *
 * @license{<blockquote>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </blockquote>}
 *
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

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