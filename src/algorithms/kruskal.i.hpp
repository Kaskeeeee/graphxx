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
Id find_representative(std::vector<std::tuple<Id, size_t>> &ranked_sets,
                       Id id) {
  Id root = std::get<0>(ranked_sets[id]);
  Id tmp;
  while (root != (tmp = std::get<0>(ranked_sets[root]))) {
    root = tmp;
  }

  while (root != id) {
    id = std::get<0>(ranked_sets[id]);
    std::get<0>(ranked_sets[id]) = root;
  }

  return root;
}

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
std::vector<Edge<G>> visit(const G &graph, Weight weight) {

  size_t size = graph.num_vertices();

  std::vector<Edge<G>> minimum_spanning_tree;
  std::vector<std::tuple<Vertex<G>, size_t>> ranked_sets(size);

  using WeightedEdge = std::tuple<Distance, Edge<G>>;
  std::vector<WeightedEdge> queue;

  // Initialize every vertex set with the vertex Id itself
  for (Vertex<G> vertex = 0; vertex < size; vertex++) {
    std::get<0>(ranked_sets[vertex]) = vertex;
    std::get<1>(ranked_sets[vertex]) = 0;
    for (auto &edge : graph[vertex]) {
      queue.push_back({weight(edge), edge});
    }
  }

  std::sort(queue.begin(), queue.end());

  // At every iteration, we want to add an edge to the tree, but we must
  // check if adding the edge will create a cycle. To do so, we analyse if
  // the vertices of the edge belong to different clusters. If yes, we can
  // safely add it. Then, we update the representative vertex of the cluster
  // of one of the two verteces
  for (auto weighted_edge : queue) {
    Edge<G> edge = std::get<1>(weighted_edge);

    Vertex<G> source = graph.get_source(edge);
    Vertex<G> target = graph.get_target(edge);

    Vertex<G> source_root = find_representative(ranked_sets, source);
    Vertex<G> target_root = find_representative(ranked_sets, target);

    if (source_root != target_root) {
      size_t source_set_rank = std::get<1>(ranked_sets[source_root]);
      size_t target_set_rank = std::get<1>(ranked_sets[target_root]);

      if (source_set_rank < target_set_rank) {
        std::get<0>(ranked_sets[source_root]) = target_root;
      } else {
        std::get<0>(ranked_sets[target_root]) = source_root;
      }

      if (source_set_rank == target_set_rank) {
        std::get<1>(ranked_sets[source_root])++;
      }

      minimum_spanning_tree.push_back(edge);
    }
  }

  return minimum_spanning_tree;
};

} // namespace graphxx::algorithms::kruskal