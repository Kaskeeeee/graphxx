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
#include "utils.hpp"

#include <limits>
#include <vector>

namespace graphxx::algorithms::bellman_ford {

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
std::vector<Node<Vertex<G>, Distance>> visit(const G &graph, Vertex<G> source,
                                             Weight weight) {
  using NodeType = Node<Vertex<G>, Distance>;
  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();
  std::vector<NodeType> distance_tree{
      graph.num_vertices(),
      NodeType{.distance = distance_upperbound, .parent = INVALID_VERTEX<G>}};

  distance_tree[source].distance = 0;

  // Relax edges |nodes| - 1 times
  for (Vertex<G> i = 0; i < graph.num_vertices() - 1; ++i) {
    bool at_least_one_edge_relaxed = false;
    for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        auto edge_weight = weight(edge);
        auto source_distance = distance_tree[graph.get_source(edge)].distance;
        auto target_distance = distance_tree[graph.get_target(edge)].distance;
        auto edge_target = graph.get_target(edge);

        if (!sum_will_overflow(source_distance, edge_weight) &&
            source_distance != distance_upperbound &&
            source_distance + edge_weight < target_distance) {
          distance_tree[edge_target].distance = source_distance + edge_weight;
          distance_tree[edge_target].parent = graph.get_source(edge);
          at_least_one_edge_relaxed = true;
        }
      }
    }
    // Exit if none of the edges has been relaxed
    if (!at_least_one_edge_relaxed) {
      break;
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      auto edge_source = graph.get_source(edge);
      auto edge_target = graph.get_target(edge);

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