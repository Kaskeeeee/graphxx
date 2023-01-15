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

#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <unordered_map>

namespace graphxx::algorithms::ford_fulkerson {

struct BFSVertex {
  VertexStatus status;
  int parent = -1;
  int edge = -1;
  int residual_capacity = -1;
};

using BFSTree = std::unordered_map<DefaultIdType, BFSVertex>;

/// @brief 
/// @tparam WeightType 
template <concepts::Numeric WeightType>
using FlowMap = std::unordered_map<DefaultIdType, WeightType>;

/// @brief 
/// @tparam WeightType 
template <concepts::Numeric WeightType> struct FFpair {
  FlowMap<WeightType> flow;
  WeightType max_flow;
};

/// @brief Implementation of ford_fulkerson algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @tparam C object that overloads operator[] to get the weight of a specific
/// edge
/// @tparam WeightType numeric weight type
/// @param graph input graph
/// @param source source vertex
/// @param sink sink vertex
/// @param edges_capacity edges capacity
/// @return maximum flow from source to sink in the given graph
template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType = DecaySubscriptValue<DefaultIdType, C>>
FFpair<WeightType> visit(const G &graph, const Vertex &source,
                         const Vertex &sink, C &&edges_capacity);

} // namespace graphxx::algorithms::ford_fulkerson

#include "algorithms/ford_fulkerson.i.hpp"

#endif