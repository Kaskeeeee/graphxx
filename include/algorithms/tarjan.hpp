/**
 * @file This file is the header of Tarjan algorithm
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include <vector> // std::vector

/// algorithms namespace contains all the algorithms available and related support structures
namespace graphxx::algorithms {

/// @brief Stucture of the node, containing informations about
struct TarjanNode {
  int32_t index;
  int32_t low_link;
  bool on_stack;
};

/// @brief a map of id to Node
using TarjanTree = std::vector<TarjanNode>;

/// @brief a vector of id
/// @tparam Id type of vertices identifier
template <concepts::Identifier Id> using StackVector = std::vector<Id>;

/// @brief a vector containing all the strongly connected components
/// @tparam Id type of vertices identifier
template <concepts::Identifier Id>
using SCCVector = std::vector<StackVector<Id>>;

/// @brief Implementation of Tarjan algorithm. Tarjan starts by performing a DFS
/// from an arbitrary start node. Then, the algorithms will
/// recover all the strongly connected components as certain subtrees fromt he
/// result of the DFS.
/// @tparam G type of input graph
/// @param graph graph on which the algorithm will run
/// @return a vector of vertices, containing all the strongly connected
/// components
template <concepts::Graph G> SCCVector<Vertex<G>> tarjan(const G &graph);

} // namespace graphxx::algorithms

#include "algorithms/tarjan.i.hpp"