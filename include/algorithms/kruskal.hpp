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
#include "utils.hpp"

#include <functional>
#include <unordered_map>
#include <vector>

namespace graphxx::algorithms::kruskal {

/// @brief flatten Tree that will collect the Edge ids of the minimum spanning
///        tree
template <concepts::Graph G> using DistanceTree = std::vector<typename G::Edge>;

/// @brief Map that divides the vertices into "clusters" and allows us to check
///        if two vertices belong to the same cluster or not and hence decide
///        whether adding an edge creates a cycle
template <concepts::Identifier Id>
using ClusterMap = std::unordered_map<Id, Id>;

/// @brief Implementation of kruskal algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
///         edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param weight edges weights
/// @return flatten tree as described for type Tree
template <
    concepts::Graph G,
    std::invocable<typename G::Edge> Weight = std::function<
        std::tuple_element_t<2, typename G::Edge>(const typename G::Edge &)>,
    typename Distance = decltype(std::declval<Weight>()(typename G::Edge{}))>
DistanceTree<G> visit(
    const G &graph,
    Weight weight = [](const G::Edge &edge) { return std::get<2>(edge); });

} // namespace graphxx::algorithms::kruskal

#include "algorithms/kruskal.i.hpp"