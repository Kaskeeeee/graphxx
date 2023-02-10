/**
 * @file This file is the header of Ford Fulkerson algorithm
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

#include "base.hpp"           // Vertex
#include "graph_concepts.hpp" // Graph

#include <functional> // std::function
#include <vector>     // std::vector

namespace graphxx::algorithms {

/// @brief Implementation of Ford Fulkerson algorithm. Ford Fulkerson performs a
/// BFS or DFS in order to find a path from the source (starting vertex) to the
/// sink (goal vertex), with available capacity on all edges in the path. If
/// there is a path, the algorithm sends flow along it, taking some of the
/// capacity of the edges. Then it finds another path, and so on. The algorithms
/// terminates when there isn't a path with enough capacity on its edges,
/// returning the maximum flow from the source to the sink.
/// @tparam G type of input graph
/// @tparam Flow type of flow and capacity of the edges
/// @param graph graph on which the algorithm will run
/// @param source starting vertex
/// @param sink goal vertex
/// @param capacity edges capacity
/// @return a number, which is the maximum flow from source to sink in the given
/// graph
template <concepts::Graph G, concepts::Numeric Flow = size_t>
Flow ford_fulkerson(const G &graph, Vertex<G> source, Vertex<G> sink,
                    std::vector<std::vector<Flow>> &capacity);

} // namespace graphxx::algorithms

#include "algorithms/ford_fulkerson.i.hpp"