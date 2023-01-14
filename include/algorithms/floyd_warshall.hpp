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

#include <unordered_map>

namespace graphxx::algorithms::floyd_warshall {

/// @brief Node containing informations about its distance and
///        and the previous node on the shortest path from a given source
/// @tparam WeightType
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  DefaultIdType parent;

  Node();
};

/// @brief A simple map of maps of ids to Nodes
/// @tparam WeightType
template <concepts::Numeric WeightType>
using Map = std::unordered_map<DefaultIdType, std::unordered_map<DefaultIdType, Node<WeightType>>>;

/// @brief Implementantation of Floyd Warhsall algorithm for multi source
///        shortest paths
/// @tparam G graph type
/// @tparam C edge weights supplier (overloads operator[])
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param weights edges weights
/// @return a map of maps containing all shortest paths
template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType = DecaySubscriptValue<DefaultIdType, C>>
Map<WeightType> visit(const G &graph, C &&weights);

} // namespace graphxx::algorithms::floyd_warshall

#include "algorithms/floyd_warshall.i.hpp"