/**
 * @file This file contains utility functions to manage graph
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
#include "graph_concepts.hpp" // concepts::Graph

#include <numeric> // std::iota
#include <vector>  // std::vector

/// utils namespace contains all the utilities functions used throughout the project
namespace graphxx::utils {

/// @brief Get vector of sorted graph vertices
/// @tparam G type of graph
/// @param graph input graph
/// @return vector that contains sorted vertices
template <concepts::Graph G>
std::vector<Vertex<G>> get_sorted_vertices(const G &graph) {
  std::vector<Vertex<G>> vertices(graph.num_vertices());
  std::iota(vertices.begin(), vertices.end(), 0);
  return vertices;
}

/// @brief Get vector of sorted graph edges
/// @tparam G type of graph
/// @param graph input graph
/// @return vector that contains sorted edges
template <concepts::Graph G>
std::vector<std::pair<Vertex<G>, Vertex<G>>> get_sorted_edges(const G &graph) {
  std::vector<std::pair<Vertex<G>, Vertex<G>>> edges;

  for (auto &out_edges : graph) {
    for (auto &edge : out_edges) {
      edges.emplace_back(graph.get_source(edge), graph.get_target(edge));
    }
  }

  std::sort(edges.begin(), edges.end());
  return edges;
}

} // namespace graphxx::utils