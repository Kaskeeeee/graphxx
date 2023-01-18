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
#include "utils.hpp"

#include <functional>
#include <vector>

namespace graphxx::algorithms::dijkstra {

/// @brief Node containing informations about the parent (which is the previous
///        node)
///        on the shortest path and its distance from the source
/// @tparam WeightType numeric weight
template <concepts::Identifier Id, concepts::Numeric Distance> struct Node {
  Distance distance;
  Id parent;
};

/// @brief flatten Tree that collects all Node structs containing information
///        based on the source vertex
///        i.e. for each vertex its distance from the source and the previous
///        node in the shortest path
/// @tparam WeightType
template <concepts::Identifier Id, concepts::Numeric Distance>
using DistanceTree = std::vector<Node<Id, Distance>>;

/// @brief Implementation of dijkstra algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
///         edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param weight edges weights
/// @return flatten tree as described for type Tree<WeightType>
template <
    concepts::Graph G,
    std::invocable<typename G::Edge> Weight = std::function<
        std::tuple_element_t<2, typename G::Edge>(const typename G::Edge &)>,
    typename Distance = decltype(std::declval<Weight>()(typename G::Edge{}))>
DistanceTree<GraphId<G>, Distance> visit(
    const G &graph, GraphId<G> source,
    Weight weight = [](const G::Edge &edge) { return std::get<2>(edge); });

} // namespace graphxx::algorithms::dijkstra

#include "algorithms/dijkstra.i.hpp"