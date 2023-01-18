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

namespace graphxx::algorithms::bellman_ford {

template <concepts::Graph G, std::invocable<typename G::Edge> Weight,
          typename Distance>
DistanceTree<GraphId<G>, Distance> visit(const G &graph, GraphId<G> source,
                                         Weight weight) {

  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();
  DistanceTree<GraphId<G>, Distance> distance_tree{
      graph.num_vertices(),
      Node{.distance = distance_upperbound, .parent = source}};

  distance_tree[source].distance = 0;

  // Relax edges |nodes| - 1 times
  for (GraphId<G> i = 0; i < graph.num_vertices() - 1; ++i) {
    for (GraphId<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        auto edge_source = graph.source(edge);
        auto edge_target = graph.target(edge);

        if (!sum_will_overflow(distance_tree[edge_source].distance,
                               weight(edge)) &&
            distance_tree[edge_source].distance + weight(edge) <
                distance_tree[edge_target].distance) {
          distance_tree[edge_target].distance =
              distance_tree[edge_source].distance + weight(edge);
          distance_tree[edge_target].parent = edge_source;
        }
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (GraphId<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      auto edge_source = graph.source(edge);
      auto edge_target = graph.target(edge);

      if (!sum_will_overflow(distance_tree[edge_source].distance,
                             weight(edge)) &&
          distance_tree[edge_source].distance + weight(edge) <
              distance_tree[edge_target].distance) {
        throw exceptions::InvariantViolationException("negative cycle found");
      }
    }
  }

  return distance_tree;
}
} // namespace graphxx::algorithms::bellman_ford