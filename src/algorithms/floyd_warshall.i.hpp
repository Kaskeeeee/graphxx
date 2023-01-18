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

template <concepts::Graph G, std::invocable<typename G::Edge> Weight,
          typename Distance>
DistanceTree<GraphId<G>, Distance> visit(const G &graph, Weight weight) {

  DistanceTree<GraphId<G>, Distance> map;

  constexpr auto distance_upperbound = std::numeric_limits<Distance>::max();

  for (GraphId<G> i = 0; i < graph.num_vertices(); i++) {
    for (GraphId<G> j = 0; j < graph.num_vertices(); j++) {
      map[i][j].distance = distance_upperbound;
      map[i][j].parent = INVALID_VERTEX<G>;
    }
  }

  for (GraphId<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      map[graph.source(edge)][graph.target(edge)].distance = weight(edge);
      map[graph.source(edge)][graph.target(edge)].parent = graph.source(edge);
    }

    for (GraphId<G> u = 0; u < graph.num_vertices(); u++) {
      for (GraphId<G> v = 0; v < graph.num_vertices(); v++) {
        for (GraphId<G> w = 0; w < graph.num_vertices(); w++) {
          if (v == w) {
            map[v][w].distance = 0;
            map[v][w].parent = INVALID_VERTEX<G>;
            continue;
          }
          auto &v_to_u = map[v][u];
          auto &u_to_w = map[u][w];
          auto &v_to_w = map[v][w];
          if (!sum_will_overflow(v_to_u.distance, u_to_w.distance) &&
              v_to_u.distance + u_to_w.distance < v_to_w.distance) {
            v_to_w.distance = v_to_u.distance + u_to_w.distance;
            v_to_w.parent = u_to_w.parent;
          }
        }
      }
    }
  }

  return map;
}

} // namespace graphxx::algorithms::floyd_warshall