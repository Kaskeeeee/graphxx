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
#include "utils.hpp"

#include <limits>

namespace graphxx::algorithms::floyd_warshall {

template <concepts::Numeric WeightType>
Node<WeightType>::Node()
    : distance{std::numeric_limits<WeightType>::max()}, parent{
                                                            INVALID_VERTEX} {};

template <concepts::Graph G, concepts::Subscriptable<DefaultIdType> C,
          concepts::Numeric WeightType>
Map<WeightType> visit(const G &graph, C &&weights) {
  Map<WeightType> map;
  for (auto edge : graph.edges()) {
    map[edge.u][edge.v].distance = weights[edge.id];
    map[edge.u][edge.v].parent = edge.u;
  }

  for (auto u : graph.vertices()) {
    for (auto v : graph.vertices()) {
      for (auto w : graph.vertices()) {
        if (v == w) {
          map[v][w].distance = 0;
          map[v][w].parent = INVALID_VERTEX;
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

  return map;
}

} // namespace graphxx::algorithms::floyd_warshall