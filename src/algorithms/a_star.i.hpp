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

#include "algorithms/a_star.hpp"
#include "base.hpp"
#include "build_path.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

namespace graphxx::algorithms::a_star {

template <concepts::Graph G, std::invocable<Vertex<G>> Heuristic,
          std::invocable<Edge<G>> Weight, typename Distance>
std::vector<Node<Vertex<G>, Distance>>
visit(const G &graph, Vertex<G> source, Vertex<G> target,
      Heuristic heuristic_weight, Weight weight) {

  using NodeType = Node<Vertex<G>, Distance>;
  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();
  std::vector<NodeType> path_vector;
  std::vector<NodeType> distance_tree;

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); ++vertex) {
    distance_tree.push_back(
        NodeType{.distance = distance_upperbound, .parent = INVALID_VERTEX<G>});
  }

  using WeightedVertex = std::tuple<Distance, Vertex<G>>;
  std::priority_queue<WeightedVertex, std::vector<WeightedVertex>,
                      std::greater<WeightedVertex>>
      queue;

  distance_tree[source].distance = 0;
  queue.push({heuristic_weight(source), source});

  while (!queue.empty()) {
    auto first_element = queue.top();
    auto u = std::get<1>(first_element);
    queue.pop();

    if (u == target) {
      return build_path(distance_tree, source, target);
    }

    for (auto edge : graph[u]) {
      auto v = graph.target(edge);
      Distance edge_weight = weight(edge);

      if (edge_weight < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      Distance alternative_distance = distance_tree[u].distance + edge_weight;
      Distance new_heuristic_distance =
          alternative_distance + heuristic_weight(v);

      if (sum_will_overflow(distance_tree[u].distance, edge_weight) ||
          sum_will_overflow(alternative_distance, heuristic_weight(v))) {
        continue;
      }

      if (alternative_distance < distance_tree[v].distance) {
        distance_tree[v].distance = alternative_distance;
        distance_tree[v].parent = u;
        queue.push({new_heuristic_distance, v});
      }
    }
  }
  return {};
}
} // namespace graphxx::algorithms::a_star