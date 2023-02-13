/**
 * @file This file is the header implementation of the random graph generator
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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

#include "generators/graph_generator.hpp" // GraphGenerator

#include <chrono>        // std::chrono::system_clock::now
#include <random>        // std::default_random_engine
#include <unordered_map> // std::unordered_map

namespace graphxx {
GraphGenerator::GraphGenerator() : _seed{0}, _fixed_seed{false} {};
GraphGenerator::GraphGenerator(unsigned int seed)
    : _seed{seed}, _fixed_seed{true} {};

template <concepts::Graph G>
void GraphGenerator::generate_random_graph(G &graph, int num_vertices,
                                           int num_edges, int max_out_degree,
                                           bool self_edges) {
  for (int i = 0; i < num_vertices; i++) {
    graph.add_vertex();
  }

  if (max_out_degree < 0) {
    max_out_degree = num_vertices;
  }

  if (!_fixed_seed) {
    _seed = get_seed();
  }

  std::default_random_engine generator(_seed);
  std::uniform_int_distribution<Vertex<G>> distribution(0, num_vertices - 1);

  std::unordered_map<Vertex<G>, int> out_degree;

  auto satisfy_conditions = [&](Vertex<G> source, Vertex<G> target) {
    bool satisfied = true;

    if (graph.has_edge(source, target))
      satisfied = false;

    if (!self_edges && source == target)
      satisfied = false;

    if (out_degree[source] >= max_out_degree)
      satisfied = false;

    if (G::DIRECTEDNESS == Directedness::UNDIRECTED &&
        out_degree[target] >= max_out_degree)
      satisfied = false;

    return satisfied;
  };

  for (int i = 0; i < num_edges; i++) {

    Vertex<G> source_id = distribution(generator);
    Vertex<G> target_id = distribution(generator);

    bool found = false;

    for (int s = 0; s <= num_vertices; s++) {
      for (int t = 0; t <= num_vertices; t++) {
        target_id = (target_id + 1) % num_vertices;

        if (satisfy_conditions(source_id, target_id)) {
          found = true;
          break;
        }
      }

      if (found)
        break;

      source_id = (source_id + 1) % num_vertices;
    }

    if (!found) {
      return;
    }

    graph.add_edge(source_id, target_id);
    out_degree[source_id]++;

    if (G::DIRECTEDNESS == Directedness::UNDIRECTED && source_id != target_id) {
      out_degree[target_id]++;
    }
  }
}

template <concepts::Graph G, concepts::Numeric W>
std::unordered_map<Edge<G>, W, XorTupleHash<Edge<G>>>
GraphGenerator::generate_random_weights(const G &graph, W min_weight,
                                        W max_weight) {
  std::unordered_map<Edge<G>, W, XorTupleHash<Edge<G>>> weights;

  if (min_weight > max_weight) {
    std::swap(min_weight, max_weight);
  }

  if (!_fixed_seed) {
    _seed = get_seed();
  }

  std::default_random_engine generator(_seed);
  std::uniform_int_distribution<W> distribution(min_weight, max_weight);

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    for (auto edge : graph[vertex]) {
      weights[edge] = distribution(generator);
    }
  }

  return weights;
}

unsigned int GraphGenerator::get_seed() {
  return std::chrono::system_clock::now().time_since_epoch().count();
}

} // namespace graphxx
