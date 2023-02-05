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

#include "algorithms/floyd_warshall.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <limits>

namespace graphxx::algorithms::floyd_warshall {

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
std::vector<std::vector<Node<Vertex<G>, Distance>>> visit(const G &graph,
                                                          Weight weight) {
  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();
  size_t num_vertices = graph.num_vertices();

  std::vector<std::vector<Node<Vertex<G>, Distance>>> matrix(
      num_vertices, std::vector<Node<Vertex<G>, Distance>>(
                        num_vertices, Node{.distance = distance_upperbound,
                                           .parent = INVALID_VERTEX<G>}));

  for (Vertex<G> u = 0; u < num_vertices; u++) {
    for (auto &edge : graph[u]) {
      Vertex<G> v = graph.get_target(edge);
      matrix[u][v].distance = weight(edge);
      matrix[u][v].parent = u;
    }
    matrix[u][u].distance = 0;
    matrix[u][u].parent = INVALID_VERTEX<G>;
  }

  for (Vertex<G> u = 0; u < num_vertices; u++) {
    for (Vertex<G> v = 0; v < num_vertices; v++) {
      for (Vertex<G> w = 0; w < num_vertices; w++) {
        auto &v_to_u = matrix[v][u];
        auto &u_to_w = matrix[u][w];
        auto &v_to_w = matrix[v][w];
        if (!sum_will_overflow(v_to_u.distance, u_to_w.distance) &&
            v_to_u.distance + u_to_w.distance < v_to_w.distance) {
          v_to_w.distance = v_to_u.distance + u_to_w.distance;
          v_to_w.parent = u_to_w.parent;
        }
      }
    }
  }

  for (Vertex<G> v = 0; v < num_vertices; v++) {
    if (matrix[v][v].distance < 0) {
      throw exceptions::InvariantViolationException(
          "negative weight cycle found");
    }
  }

  return matrix;
}

} // namespace graphxx::algorithms::floyd_warshall