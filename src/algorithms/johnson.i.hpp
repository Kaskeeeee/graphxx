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
#include "algorithms/dijkstra.hpp"
#include "algorithms/johnson.hpp"
#include "base.hpp"

namespace graphxx::algorithms::johnson {

template <concepts::Graph G, std::invocable<typename G::Edge> Weight,
          typename Distance>
DistanceTree<typename G::Id, Distance> visit(G &graph, Weight weight) {

  using Vertex = typename G::Id;

  DistanceTree<Vertex, Distance> map;

  // New vertex
  Vertex q = graph.num_vertices();
  graph.add_vertex(q);

  WeightMap<G, Distance> weight_map;

  auto get_weight = [&](typename G::Edge e) {
    return weight_map[{graph.source(e), graph.target(e)}];
  };

  // Add new edge from q to every other vertex
  for (Vertex vertex = 0; vertex < graph.num_vertices(); vertex++) {
    graph.add_edge(q, vertex);
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      weight_map[edge] = weight(edge);
    }
  }

  // Run Bellman–Ford algorithm
  auto bf_tree = bellman_ford::visit(graph, q, weight);

  // Reweigh the edges using the values computed by Bellman–Ford algorithm:
  // w(u,v) = w(u,v) + h(u) - h(v)
  for (Vertex vertex = 0; vertex < graph.size(); vertex++) {
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      weight_map[edge] += bf_tree[graph.source(edge)].distance -
                          bf_tree[graph.target(edge)].distance;
    }
  }

  // Remove vertex q
  graph.remove_vertex(q);

  // Run Dijkstra for every vertex
  for (Vertex vertex_source = 0; vertex_source < graph.num_vertices();
       vertex_source++) {
    auto d_tree = dijkstra::visit(graph, vertex_source, get_weight);
    std::vector<Node<Vertex, Distance>> temp;
    for (Vertex vertex_target = 0; vertex_target < graph.num_vertices();
         vertex_target++) {
      temp.push_back({.distance = d_tree[vertex_target].distance +
                                  bf_tree[vertex_target].distance -
                                  bf_tree[vertex_source].distance,
                      .parent = d_tree[vertex_target].parent});
    }
    map.push_back(temp);
  }

  return map;
}
} // namespace graphxx::algorithms::johnson