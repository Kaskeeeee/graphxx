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
#include <vector>

namespace graphxx::algorithms::dfs {

/// @brief Structure to store information about graph vertices during the visit
template <concepts::Identifier Id>
struct Node {
  /// @brief Vertex visitation status
  VertexStatus status;
  /// @brief Id of the predecessor Vertex in DFS Tree
  Id parent;
  /// @brief Counter indicating when vertex is discovered
  int discovery_time;
  /// @brief Counter indicating when the processing of vertex is finished
  int finishing_time;
};

/// @brief flatten Tree that collects all DFSVertex structs generated during the
///        visit
template <concepts::Identifier Id> using DistanceTree = std::vector<Node<Id>>;

/// @brief Performs a depth-first traversal of the graph. A depth-first
///        traversal chooses a vertex adjacent to the current vertex to visit
///        next. If all adjacent vertices have already been discovered, or there
///        are no adjacent vertices, then the algorithm backtracks to the last
///        vertex that had undiscovered neighbors.
/// @tparam G type of input graph
/// @param graph input graph
/// @param source vertex
/// @return flatten tree as described for type DFSTree
template <concepts::Graph G>
DistanceTree<typename G::Id> visit(const G &graph, typename G::Id source);

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
DistanceTree<typename G::Id>
visit(const G &graph, typename G::Id source,
      const std::function<void(typename G::Id)> &callback);

} // namespace graphxx::algorithms::dfs

#include "algorithms/dfs.i.hpp"