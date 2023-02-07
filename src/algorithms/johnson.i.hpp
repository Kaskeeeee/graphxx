/**
 * @file This file is the header implementation of Johnson algorithm
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
#include "graph_concepts.hpp"

namespace graphxx::algorithms {

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
std::vector<std::vector<JohnsonNode<Vertex<G>, Distance>>>
johnson(G &graph, Weight weight) {

  // New vertex
  size_t size = graph.num_vertices();
  Vertex<G> johnson_vertex = size;
  graph.add_vertex(johnson_vertex);

  std::vector<std::vector<JohnsonNode<Vertex<G>, Distance>>> distances(size +
                                                                       1);
  WeightMap<G, Distance> weight_map(size + 1);

  auto get_weight = [&](Edge<G> e) {
    return weight_map[graph.get_source(e)][graph.get_target(e)];
  };

  // Add new edge from johnson_vertex to every other vertex
  for (Vertex<G> vertex = 0; vertex < johnson_vertex; vertex++) {
    graph.add_edge(johnson_vertex, vertex);
    for (auto &&edge : graph[vertex]) {
      auto source = graph.get_source(edge);
      auto target = graph.get_target(edge);
      weight_map[source][target] = weight(edge);
    }
  }

  for (auto &&edge : graph[johnson_vertex]) {
    auto source = graph.get_source(edge);
    auto target = graph.get_target(edge);
    weight_map[source][target] = 0;
  }

  // Run Bellman–Ford algorithm
  auto bf_tree = bellman_ford(graph, johnson_vertex, weight);
  graph.remove_vertex(johnson_vertex);

  // Reweigh the edges using the values computed by Bellman–Ford algorithm:
  // w(u,v) = w(u,v) + h(u) - h(v)
  for (Vertex<G> vertex = 0; vertex < size; vertex++) {
    for (auto &&edge : graph[vertex]) {
      Vertex<G> source = graph.get_source(edge);
      Vertex<G> target = graph.get_target(edge);
      weight_map[source][target] +=
          bf_tree[source].distance - bf_tree[target].distance;
    }
  }

  // Run Dijkstra for every vertex
  for (Vertex<G> source = 0; source < size; source++) {
    auto d_tree = dijkstra(graph, source, get_weight);
    for (Vertex<G> target = 0; target < size; target++) {
      distances[source].push_back({.distance = d_tree[target].distance +
                                               bf_tree[target].distance -
                                               bf_tree[source].distance,
                                   .parent = d_tree[target].parent});
    }
  }

  return distances;
}
} // namespace graphxx::algorithms