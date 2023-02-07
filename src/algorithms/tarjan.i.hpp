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

#include "algorithms/tarjan.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <algorithm>
#include <vector>

namespace graphxx::algorithms {

namespace detail::tarjan {
template <concepts::Graph G>
void tarjan_rec(const G &graph, Vertex<G> v, TarjanTree &tarjan_tree,
                SCCVector<Vertex<G>> &scc_vector, StackVector<Vertex<G>> &stack,
                int &index) {
  tarjan_tree[v].index = index;
  tarjan_tree[v].low_link = index;
  ++index;
  stack.push_back(v);
  tarjan_tree[v].on_stack = true;

  for (auto edge : graph[v]) {
    auto target = graph.get_target(edge);
    if (tarjan_tree[target].index == -1) {
      tarjan_rec(graph, target, tarjan_tree, scc_vector, stack, index);
      tarjan_tree[v].low_link =
          std::min(tarjan_tree[v].low_link, tarjan_tree[target].low_link);
    } else if (tarjan_tree[target].on_stack) {
      tarjan_tree[v].low_link =
          std::min(tarjan_tree[v].low_link, tarjan_tree[target].index);
    }
  }
  if (tarjan_tree[v].low_link == tarjan_tree[v].index) {
    std::vector<Vertex<G>> new_scc;

    auto w = stack.back();
    stack.pop_back();
    tarjan_tree[w].on_stack = false;
    new_scc.push_back(w);

    while (w != v) {
      w = stack.back();
      stack.pop_back();
      tarjan_tree[w].on_stack = false;
      new_scc.push_back(w);
    }

    scc_vector.push_back(new_scc);
  }
}
} // namespace detail::tarjan

template <concepts::Graph G> SCCVector<Vertex<G>> tarjan(const G &graph) {

  SCCVector<Vertex<G>> scc_vector;
  TarjanTree tarjan_tree;
  StackVector<Vertex<G>> stack;
  int index = 0;

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    tarjan_tree.push_back(
        TarjanNode{.index = -1, .low_link = -1, .on_stack = false});
  }

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    if (tarjan_tree[vertex].index != -1) {
      continue;
    }
    detail::tarjan::tarjan_rec(graph, vertex, tarjan_tree, scc_vector, stack,
                               index);
  }

  return scc_vector;
}

} // namespace graphxx::algorithms