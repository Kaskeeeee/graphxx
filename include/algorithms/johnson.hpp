/**
 * @file This file is the header of Johnson algorithm
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "base.hpp"           // Edge
#include "graph_concepts.hpp" // Graph

#include <concepts>      // std::invocable
#include <functional>    // std::function
#include <tuple>         // std::tuple_element_t
#include <unordered_map> // std::unordered_map
#include <utility>       // std::declval
#include <vector>        // std::vector

namespace graphxx::algorithms {

/// @brief Stucture of the node, containing informations about the parent (which
/// is the previous node) on the shortest path and its distance from the source
/// @tparam Id type of vertices identifier
/// @tparam Distance type of distance among the nodes
template <concepts::Identifier Id, concepts::Numeric Distance>
struct JohnsonNode {
  /// @brief Distance of the vertex from the source
  Distance distance;
  /// @brief Id of the predecessor vertex in the visited tree
  Id parent;
};

/// @tparam G type of input graph
/// @tparam Distance type of distance among the nodes
template <concepts::Graph G, concepts::Numeric Distance>
using WeightMap = std::vector<std::unordered_map<Vertex<G>, Distance>>;

/// @brief Implementation of Johnson algorithm. Johnson starts by placing a new
/// vertex in the graph and adding an edge from it to all other vetices of the
/// graph. Then it uses the Bellman Ford algorithm from the new vertex q, to
/// find for each vertex v the minimum weight h(v) of a path from q to v. After
/// reweighting the the edges of the original graph using the values computed by
/// the Bellman Ford algorithm, q is removed and Dijkstra algorithm is used
/// to find the shortest paths from each node s to every other vertex in the
/// reweighted graph.
/// @tparam G type of input graph
/// @tparam Weight function used to get weight of an edge
/// @tparam Distance type of distance among the nodes
/// @param graph graph on which the algorithm will run
/// @param weight weight function
/// @return a vector of vectors composed by JohnsonNode structs, containing all
/// shortest paths
template <concepts::Graph G,
          std::invocable<Edge<G>> Weight =
              std::function<std::tuple_element_t<2, Edge<G>>(const Edge<G> &)>,
          typename Distance = decltype(std::declval<Weight>()(Edge<G>{}))>
std::vector<std::vector<JohnsonNode<Vertex<G>, Distance>>> johnson(
    G &graph,
    Weight weight = [](const Edge<G> &edge) { return std::get<2>(edge); });

} // namespace graphxx::algorithms

#include "algorithms/johnson.i.hpp"