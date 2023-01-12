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

#include "algorithms/bellman_ford.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"

#include <limits>

namespace graph::algorithms::bellman_ford {

template <concepts::Numeric WeightType>
Node<WeightType>::Node()
    : distance{std::numeric_limits<WeightType>::max()}, parent{
                                                            INVALID_VERTEX} {};

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Tree<WeightType> visit(const G &graph, const Vertex &source,
                       C &&edges_weights) {
  Tree<WeightType> tree;
  tree[source].distance = 0;
  tree[source].parent = INVALID_VERTEX;

  // Relax edges |nodes| - 1 times
  for (int64_t i = 0; i < static_cast<int64_t>(graph.vertices().size() - 1);
       ++i) {
    for (auto edge : graph.edges()) {
      auto &edge_source = tree[edge.u];
      auto &edge_target = tree[edge.v];

      if (!sum_will_overflow(edge_source.distance, edges_weights[edge]) &&
          edge_source.distance + edges_weights[edge] < edge_target.distance) {
        edge_target.distance = edge_source.distance + edges_weights[edge];
        edge_target.parent = edge.u;
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (auto edge : graph.edges()) {
    auto &edge_source = tree[edge.u];
    auto &edge_target = tree[edge.v];

    if (!sum_will_overflow(edge_source.distance, edges_weights[edge]) &&
        edge_source.distance + edges_weights[edge] < edge_target.distance) {
      throw exceptions::InvariantViolationException("negative cycle found");
    }
  }

  return tree;
}
} // namespace graph::algorithms::bellman_ford