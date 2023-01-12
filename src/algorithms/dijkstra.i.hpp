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

#include "algorithms/dijkstra.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace graph::algorithms::dijkstra {

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights) {
  Tree<WeightType> tree;
  auto distance_upperbound = std::numeric_limits<WeightType>::max();

  using VertexDistancePair = std::pair<Vertex, WeightType>;
  constexpr auto COMPARATOR = [&](const VertexDistancePair &p,
                                  const VertexDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexDistancePair, std::vector<VertexDistancePair>,
                      decltype(COMPARATOR)>
      queue{COMPARATOR};

  tree[source] = {.distance = 0, .parent = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : graph.vertices()) {
    if (vertex == source) {
      continue;
    }
    tree[vertex] =
        Node{.distance = distance_upperbound, .parent = INVALID_VERTEX};
  }

  while (!queue.empty()) {
    auto u = queue.top().first;
    queue.pop();

    for (auto edge : graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = tree[u].distance + weights[edge];
      if (!sum_will_overflow(tree[u].distance, weights[edge]) &&
          alternative_distance < tree[edge.v].distance) {
        tree[edge.v].distance = alternative_distance;
        tree[edge.v].parent = u;
        queue.push(std::make_pair(edge.v, alternative_distance));
      }
    }
  }

  return tree;
}

} // namespace graph::algorithms::dijkstra