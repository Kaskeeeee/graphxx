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

#include "algorithms/dfs.hpp"
#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>
#include <vector>

namespace graphxx::algorithms::dfs {

template <concepts::Graph G>
std::vector<Node<Vertex<G>>> visit(const G &graph, Vertex<G> source) {
  return visit(graph, source, [](Vertex<G>) {});
}

template <concepts::Graph G>
std::vector<Node<Vertex<G>>>
visit(const G &graph, Vertex<G> source,
      const std::function<void(Vertex<G>)> &callback) {

  using NodeType = Node<Vertex<G>>;
  std::vector<NodeType> distance_tree;

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); ++vertex) {
    distance_tree.push_back(NodeType{.status = VertexStatus::READY,
                                     .parent = INVALID_VERTEX<G>,
                                     .discovery_time = -1,
                                     .finishing_time = -1});
  }

  int time = 0;

  visit_rec(graph, source, callback, time, distance_tree);

  return distance_tree;
}

template <concepts::Graph G>
void visit_rec(const G &graph, Vertex<G> vertex,
               const std::function<void(Vertex<G>)> &callback, int &time,
               std::vector<Node<Vertex<G>>> &distance_tree) {
  callback(vertex);

  distance_tree[vertex].status = VertexStatus::WAITING;
  distance_tree[vertex].discovery_time = ++time;

  for (auto edge : graph[vertex]) {
    Vertex<G> adjacent = graph.get_target(edge);

    if (distance_tree[adjacent].status == VertexStatus::READY) {
      distance_tree[adjacent].parent = vertex;
      visit_rec(graph, adjacent, callback, time, distance_tree);
    }
  }

  distance_tree[vertex].status = VertexStatus::PROCESSED;
  distance_tree[vertex].finishing_time = ++time;
}

} // namespace graphxx::algorithms::dfs