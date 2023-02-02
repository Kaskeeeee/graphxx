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

#include <unordered_map>

namespace graphxx {

class GraphGenerator {
public:
  GraphGenerator();
  GraphGenerator(unsigned int seed);

  template <concepts::Graph G>
  void generate_random_graph(G &graph, int num_vertices, int num_edges,
                             int max_out_degree = -1, bool self_edges = true);

  template <concepts::Graph G, concepts::Numeric W>
  std::unordered_map<DefaultIdType, W> generate_random_weights(const G &graph,
                                                    W min_weight, W max_weight);

private:
  unsigned int _seed;
  bool _fixed_seed;

  unsigned int get_seed();
};

} // namespace graph

#include "generators/graph_generator.i.hpp"