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

#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <vector>

namespace graphxx::algorithms {

template <concepts::Identifier Id, concepts::Numeric Distance>
struct BellmanFordNode {
  Distance distance;
  Id parent;
};

/// @brief Implementation of bellman_ford algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
///         edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param edges_weights edges weights
/// @return flatten tree as described for type Tree<WeightType>
template <concepts::Graph G,
          std::invocable<Edge<G>> Weight =
              std::function<std::tuple_element_t<2, Edge<G>>(const Edge<G> &)>,
          typename Distance = decltype(std::declval<Weight>()(Edge<G>{}))>
std::vector<BellmanFordNode<Vertex<G>, Distance>> bellman_ford(
    const G &graph, Vertex<G> source,
    Weight weight = [](const Edge<G> &edge) { return std::get<2>(edge); });

} // namespace graphxx::algorithms

#include "algorithms/bellman_ford.i.hpp"