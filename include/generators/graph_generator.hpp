/**
 * @file This file is the header of the class that generates random graphs
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

#include "base.hpp"           // Edge
#include "graph_concepts.hpp" // Graph
#include "tuple_utils.hpp"    // XorTupleHash

#include <unordered_map> // std::unordered_map

namespace graphxx {

/// @brief This class allows to generate graphs at runtime by specifying the
/// number of edges and vertices
class GraphGenerator {
public:
  /// @brief Graph generator constructor
  GraphGenerator();

  /// @brief Creates a graph generator with a user specified seed
  /// @param seed Seed of the random generator
  explicit GraphGenerator(unsigned int seed);

  /// @brief Generates random graph by specifying the number of edges and
  /// vertices
  /// @tparam G type of random graph
  /// @param graph refrence to output graph
  /// @param num_vertices number of graph vertices
  /// @param num_edges number of graph edges
  /// @param max_out_degree maximum out degree of graph vertices
  /// @param self_edges if a vertex can have self edges
  template <concepts::Graph G>
  void generate_random_graph(G &graph, int num_vertices, int num_edges,
                             int max_out_degree = -1, bool self_edges = true);

  /// @brief Generates random weight map for graph
  /// @tparam G type of graph
  /// @tparam W type of weight
  /// @param graph input graph
  /// @param min_weight minimum weight value
  /// @param max_weight maximum weight value
  /// @return map that assigns random weight to each edge of the graph
  template <concepts::Graph G, concepts::Numeric W>
  std::unordered_map<Edge<G>, W, XorTupleHash<Edge<G>>>
  generate_random_weights(const G &graph, W min_weight, W max_weight);

private:
  unsigned int _seed;
  bool _fixed_seed;

  /// @brief Retrives the generator seed
  /// @return Seed for the random generator
  unsigned int get_seed();
};

} // namespace graphxx

#include "generators/graph_generator.i.hpp"