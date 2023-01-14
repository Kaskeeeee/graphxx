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
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <queue>
#include <vector>

namespace graphxx::algorithms::a_star {

template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights,
                       C &&heuristic_weights) {
  Tree<WeightType> tree;
  auto distance_upperbound = std::numeric_limits<WeightType>::max();

  using VertexHeuristicDistancePair = std::pair<Vertex, WeightType>;
  constexpr auto COMPARATOR = [&](const VertexHeuristicDistancePair &p,
                                  const VertexHeuristicDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexHeuristicDistancePair,
                      std::vector<VertexHeuristicDistancePair>,
                      decltype(COMPARATOR)>
      queue{COMPARATOR};

  tree[source] = {.distance = 0,
                  .heuristic_distance = heuristic_weights[source],
                  .parent = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : graph.vertices()) {
    if (vertex == source) {
      continue;
    }
    tree[vertex] = Node{.distance = distance_upperbound,
                        .heuristic_distance = heuristic_weights[vertex],
                        .parent = INVALID_VERTEX};
    queue.push(std::make_pair(vertex, distance_upperbound));
  }

  // in this implementation, queue keeps also useless elements
  // this is due to the fact that you can't update order in the queue
  // without pushing a new element
  while (!queue.empty()) {
    // extract node and distance of min distance node
    auto next_pair = queue.top();

    // if the distance is not the one up to date, keep on popping elements
    // WARNING: to avoid undefined behavior we pop from the queue only if
    // it still has elements inside, otherwise there may be an infinite loop
    for (int64_t i = 0; i < queue.size() - 1; ++i) {
      if (tree[next_pair.first].heuristic_distance == next_pair.second) {
        break;
      }

      queue.pop();
      next_pair = queue.top();
    }

    queue.pop();

    [[unlikely]] if (tree[next_pair.first].heuristic_distance !=
                     next_pair.second) {
      return tree;
    }

    auto [u, _] = next_pair;

    for (auto edge : graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = tree[u].distance + weights[edge];
      auto new_heuristic_distance =
          alternative_distance + heuristic_weights[edge.v];
      if (!sum_will_overflow(tree[u].distance, weights[edge]) &&
          !sum_will_overflow(alternative_distance, heuristic_weights[edge.v]) &&
          alternative_distance < tree[edge.v].distance) {
        tree[edge.v].distance = alternative_distance;
        tree[edge.v].parent = u;
        tree[edge.v].heuristic_distance = new_heuristic_distance;
        queue.push(std::make_pair(edge.v, new_heuristic_distance));
      }
    }
  }
  return tree;
}
} // namespace graphxx::algorithms::a_star