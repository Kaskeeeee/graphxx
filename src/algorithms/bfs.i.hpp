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

#include "algorithms/bfs.hpp"
#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace graphxx::algorithms::bfs {

template <concepts::Graph G>
DistanceTree<typename G::Id> visit(const G &graph, typename G::Id source) {
  return visit(graph, source, [](typename G::Id) {});
}

template <concepts::Graph G>
DistanceTree<typename G::Id>
visit(const G &graph, typename G::Id source,
      const std::function<void(typename G::Id)> &callback) {

  using Vertex = typename G::Id;

  DistanceTree<Vertex> distance_tree;
  constexpr auto distance_upperbound = std::numeric_limits<size_t>::max();

  for (Vertex vertex = 0; vertex < graph.num_vertices(); ++vertex) {
    distance_tree.push_back(Node{.status = VertexStatus::READY,
                                 .distance = distance_upperbound,
                                 .parent = vertex});
  }

  distance_tree[source].status = VertexStatus::WAITING;
  distance_tree[source].distance = 0;

  std::queue<Vertex> queue;
  queue.push(source);

  while (!queue.empty()) {
    Vertex vertex_id = queue.front();
    queue.pop();

    callback(vertex_id);

    for (auto edge : graph[vertex_id]) {
      Vertex adjacent = graph.target(edge);

      if (distance_tree[adjacent].status == VertexStatus::READY) {
        distance_tree[adjacent].status = VertexStatus::WAITING;
        distance_tree[adjacent].distance =
            distance_tree[vertex_id].distance + 1;
        distance_tree[adjacent].parent = vertex_id;
        queue.push(adjacent);
      }
    }

    distance_tree[vertex_id].status = VertexStatus::PROCESSED;
  }

  return distance_tree;
}

} // namespace graphxx::algorithms::bfs