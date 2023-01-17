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

#include <vector>

namespace graphxx::algorithms::tarjan {

/// @brief Vertex containing informations for the algorithm
struct Node {
  int index;
  int low_link;
  bool on_stack;
};

/// @brief a map of id to Node
using TarjanTree = std::vector<Node>;

/// @brief a vector of id
template <concepts::Identifier Id> using StackVector = std::vector<Id>;

/// @brief a vector containing all the strongly connected components
template <concepts::Identifier Id>
using SCCVector = std::vector<StackVector<Id>>;

/// @brief Implementation of tarjan algorithm
/// @tparam G graph type that is coherent with Graph concept
/// @return a vector containing all the strongly connected components
template <concepts::Graph G> SCCVector<typename G::Id> visit(const G &graph);

} // namespace graphxx::algorithms::tarjan

#include "algorithms/tarjan.i.hpp"