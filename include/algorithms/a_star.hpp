/**
 * @file This file is the header of A* algorithm
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

#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <vector>

namespace graphxx::algorithms {

/// @brief Stucture of the node, containing informations about the parent (which
/// is the previous node) on the shortest path and its distance from the source
/// @tparam Id type of vertices identifier
/// @tparam Distance type of distance among the nodes
template <concepts::Identifier Id, concepts::Numeric Distance>
struct AStarNode {
  /// @brief Distance of the vertex from the source
  Distance distance;
  /// @brief Id of the predecessor vertex in the visited tree
  Id parent;
};

/// @brief Implementation of A* search algorithm. A* performs a best-first
/// search, visiting at each iteration the vertex with the lowest f(n) = g(n) +
/// h(n), where n is the next node on the path, g(n) is the cost of the path
/// from the start node to n and h(n) is a heuristic function that estimates the
/// cost of the cheapest path from n to the goal.
/// @tparam G type of input graph
/// @tparam Heuristic function used to get the heuristic weight of a node
/// @tparam Weight function used to get weight of an edge
/// @tparam Distance type of distance among the nodes
/// @param graph graph on which the algorithm will run
/// @param source starting vertex
/// @param target goal vertex
/// @param heuristic_weight heuristic function
/// @param weight weight function
/// @return a vector composed by AStarNode structs
template <concepts::Graph G, std::invocable<Vertex<G>> Heuristic,
          std::invocable<Edge<G>> Weight =
              std::function<std::tuple_element_t<2, Edge<G>>(const Edge<G> &)>,
          typename Distance = decltype(std::declval<Weight>()(Edge<G>{}))>
std::vector<AStarNode<Vertex<G>, Distance>> a_star(
    const G &graph, Vertex<G> source, Vertex<G> target,
    Heuristic heuristic_weight,
    Weight weight = [](const Edge<G> &edge) { return std::get<2>(edge); });
} // namespace graphxx::algorithms

#include "algorithms/a_star.i.hpp"