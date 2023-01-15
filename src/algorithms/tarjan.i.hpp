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

#if 0

#include "algorithms/tarjan.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <algorithm>

namespace graphxx::algorithms::tarjan {

SCCVector _scc_vector;
TarjanTree _tarjan_tree;
StackVector _stack;
int index;

template <concepts::Graph G> void tarjan_rec(const G &graph, Vertex v) {
  _tarjan_tree[v].index = index;
  _tarjan_tree[v].low_link = index;
  ++index;
  _stack.push_back(v);
  _tarjan_tree[v].on_stack = true;

  for (auto edge : graph.out_edges(v)) {
    if (_tarjan_tree[edge.v].index == -1) {
      tarjan_rec(graph, edge.v);
      _tarjan_tree[v].low_link =
          std::min(_tarjan_tree[v].low_link, _tarjan_tree[edge.v].low_link);
    } else if (_tarjan_tree[edge.v].on_stack) {
      _tarjan_tree[v].low_link =
          std::min(_tarjan_tree[v].low_link, _tarjan_tree[edge.v].index);
    }
  }
  if (_tarjan_tree[v].low_link == _tarjan_tree[v].index) {
    std::vector<DefaultIdType> new_scc;

    auto w = _stack.back();
    _stack.pop_back();
    _tarjan_tree[w].on_stack = false;
    new_scc.push_back(w);

    while (w != v) {
      w = _stack.back();
      _stack.pop_back();
      _tarjan_tree[w].on_stack = false;
      new_scc.push_back(w);
    }

    _scc_vector.push_back(new_scc);
  }
}

template <concepts::Graph G> SCCVector visit(const G &graph) {
  _scc_vector.clear();
  _tarjan_tree.clear();
  _stack.clear();
  index = 0;

  for (auto vertex : graph.vertices()) {
    _tarjan_tree[vertex] = TarjanVertex{};
  }

  for (auto vertex : graph.vertices()) {
    if (_tarjan_tree[vertex].index != -1) {
      continue;
    }
    tarjan_rec(graph, vertex);
  }

  return _scc_vector;
}

} // namespace graphxx::algorithms::tarjan

#endif