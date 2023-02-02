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
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <limits>

namespace graphxx::algorithms::floyd_warshall {

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
DistanceMatrix<Vertex<G>, Distance> visit(const G &graph, Weight weight) {
  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();
  DistanceMatrix<Vertex<G>, Distance> matrix;

  Vertex<G> u = 0;

  for (auto &out_edge_list : graph) {
    matrix.emplace_back();
    for (auto &&edge : out_edge_list) {
      Vertex<G> v = graph.get_target(edge);
      fill_vector(
          matrix[u], v,
          Node{.distance = distance_upperbound, .parent = INVALID_VERTEX<G>});
      matrix[u][v].distance = weight(edge);
      matrix[u][v].parent = u;
    }
    fill_vector(
        matrix[u], graph.num_vertices(),
        Node{.distance = distance_upperbound, .parent = INVALID_VERTEX<G>});
    u++;
  }

  for (Vertex<G> u = 0; u < graph.num_vertices(); u++) {
    for (Vertex<G> v = 0; v < graph.num_vertices(); v++) {
      for (Vertex<G> w = 0; w < graph.num_vertices(); w++) {
        if (v == w) {
          matrix[v][w].distance = 0;
          matrix[v][w].parent = INVALID_VERTEX<G>;
          continue;
        }
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

  return matrix;
}

} // namespace graphxx::algorithms::floyd_warshall