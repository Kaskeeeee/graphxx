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

#if 0

#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graphxx::algorithms::a_star {

/// @brief Node containing informations about its distance,
///        its heurisitc_distance (initially given by the user) and
///        the previous node in the shortest path from a given source
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType> struct Node {
  WeightType distance;
  WeightType heuristic_distance;
  DefaultIdType parent;
};

/// @brief Flatten Tree that collects all Node structs containing information
///        based on the source vertex
///        i.e. for each vertex its distance and the heuristic one from
///        respectively the source and the previous node in the shortest path
/// @tparam WeightType numeric weight
template <concepts::Numeric WeightType>
using Tree = std::unordered_map<DefaultIdType, Node<WeightType>>;

/// @brief Implementation of a_star algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param edges_weights edges weights
/// @param heuristic_weights heuristic distances for each vertex
/// @return flatten tree as described for type Tree<WeightType>
template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType = DecaySubscriptValue<DefaultIdType, C>>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&edges_weights,
                       C &&heuristic_weights);

} // namespace graphxx::algorithms::a_star

#include "algorithms/a_star.i.hpp"

#endif