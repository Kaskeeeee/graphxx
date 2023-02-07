/**
 * @file This file is the header of DFS algorithm
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

#include "algorithms_base.hpp" // VertexStatus
#include "base.hpp"            // Vertex
#include "graph_concepts.hpp"  // Graph

#include <functional> // std::function
#include <vector>     // std::vector

namespace graphxx::algorithms {

/// @brief Stucture of the node, containing informations about the parent (which
/// is the previous node) on the shortest path, its distance from the source,
/// the status of the node itself and two numbers indicating its discovery and
/// finishing time
/// @tparam Id type of vertices identifier
template <concepts::Identifier Id> struct DfsNode {
  /// @brief Vertex visitation status
  VertexStatus status;
  /// @brief Id of the predecessor vertex in the visited tree
  Id parent;
  /// @brief Counter indicating when the vertex is discovered
  size_t discovery_time;
  /// @brief Counter indicating when the processing of the vertex is finished
  size_t finishing_time;
};

/// @brief Performs a depth-first traversal of the graph. A depth-first
///        traversal chooses a vertex adjacent to the current vertex to visit
///        next. If all adjacent vertices have already been discovered, or there
///        are no adjacent vertices, then the algorithm backtracks to the last
///        vertex that had undiscovered neighbors.
/// @tparam G type of input graph
/// @param graph graph on which the algorithm will run
/// @param source starting vertex
/// @return flatten tree composed by DfsNode structs
template <concepts::Graph G>
std::vector<DfsNode<Vertex<G>>> dfs(const G &graph, Vertex<G> source);

/// @brief Performs a depth-first traversal of the graph. A depth-first
///        traversal chooses a vertex adjacent to the current vertex to visit
///        next. If all adjacent vertices have already been discovered, or there
///        are no adjacent vertices, then the algorithm backtracks to the last
///        vertex that had undiscovered neighbors. For each visited node
///        function `callback` is called.
/// @tparam G type of input graph
/// @param graph graph on which the algorithm will run
/// @param source starting vertex
/// @param callback function to call when a new node is visited
/// @return flatten tree composed by DfsNode structs
template <concepts::Graph G>
std::vector<DfsNode<Vertex<G>>>
dfs(const G &graph, Vertex<G> source,
    const std::function<void(Vertex<G>)> &callback);

} // namespace graphxx::algorithms

#include "algorithms/dfs.i.hpp"