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

#include "algorithms/kruskal.hpp"
#include "base.hpp"

#include <queue>

namespace graphxx::algorithms::kruskal {

// Search for the repserentative vertex of the cluster, which is that element in
// the map with same key and value
template <concepts::Identifier Id>
Id find_representative(std::vector<Id> &cluster_map, Id id) {
  while (id != cluster_map[id]) {
    Id tmp = cluster_map[id];
    cluster_map[id] = cluster_map[tmp];
    id = cluster_map[tmp];
  }
  return id;
}

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
std::vector<Edge<G>> visit(const G &graph, Weight weight) {

  std::vector<Edge<G>> distance_tree;
  std::vector<Vertex<G>> cluster_map;
  std::vector<int> rank;

  using WeightedEdge = std::tuple<Distance, Edge<G>>;
  std::priority_queue<WeightedEdge, std::vector<WeightedEdge>,
                      std::greater<WeightedEdge>>
      queue;

  // Initialize every vertex cluster with the vertex Id itself
  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    cluster_map.emplace_back(vertex);
    rank.push_back(0);
    for (auto &edge : graph[vertex]) {
      queue.push({weight(edge), edge});
    }
  }

  // At every iteration, we want to add an edge to the tree, but we must check
  // if adding the edge will create a cycle. To do so, we analyse if the
  // vertices of the edge belong to different clusters. If yes, we can safely
  // add it. Then, we update the representative vertex of the cluster of one of
  // the two verteces
  while (!queue.empty()) {
    Edge<G> edge = std::get<1>(queue.top());
    queue.pop();

    Vertex<G> source = graph.source(edge);
    Vertex<G> target = graph.target(edge);

    if (find_representative(cluster_map, source) !=
        find_representative(cluster_map, target)) {
      distance_tree.push_back(edge);

      if (rank[source] > rank[target]) {
        cluster_map[target] = cluster_map[source];
        rank[source] += rank[target];
      } else {
        cluster_map[source] = cluster_map[target];
        rank[target] += rank[source];
      }
    }
  }

  return distance_tree;
};

} // namespace graphxx::algorithms::kruskal