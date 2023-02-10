/**
 * @file This file is the header of Kruskal algorithm
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

#include <concepts>   // std::invocable
#include <functional> // std::function
#include <tuple>      // std::tuple_element_t
#include <utility>    // std::declval
#include <vector>     // std::vector

namespace graphxx::algorithms {

/// @brief Implementation of Kruskal algorithm. Starting by creating a forest
/// (a set of trees), where each vertex in the graph is a separate tree, Kruskal
/// iters on each edge, in increasing weight order. If the edge connects two
/// different trees then the algorithms adds it to the forest , combining two
/// trees into a single tree. At the termination of the algorithm, the forest
/// forms a minimum spanning forest or tree.
/// @tparam G type of input graph
/// @tparam Weight function used to get weight of an edge
/// @tparam Distance type of distance among the nodes
/// @param graph graph on which the algorithm will run
/// @param weight weight function
/// @return a vector of edges
template <concepts::Graph G,
          std::invocable<Edge<G>> Weight =
              std::function<std::tuple_element_t<2, Edge<G>>(const Edge<G> &)>,
          typename Distance = decltype(std::declval<Weight>()(Edge<G>{}))>
std::vector<Edge<G>> kruskal(
    const G &graph,
    Weight weight = [](const Edge<G> &edge) { return std::get<2>(edge); });

} // namespace graphxx::algorithms

#include "algorithms/kruskal.i.hpp"