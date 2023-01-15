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

#include "algorithms/dfs.hpp"
#include "algorithms_base.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <functional>

namespace graphxx::algorithms::dfs {

template <concepts::Graph G> DFSTree visit(const G &graph, Vertex source) {
  return visit(graph, source, [](Vertex) {});
}

template <concepts::Graph G>
DFSTree visit(const G &graph, Vertex source,
              const std::function<void(Vertex)> &callback) {
  DFSTree tree;
  int time = 0;
  for (Vertex vertex : graph.vertices()) {
    tree[vertex] = DFSVertex{VertexStatus::READY};
  }

  visit_rec(graph, source, callback, time, tree);

  return tree;
}

template <concepts::Graph G>
void visit_rec(const G &graph, Vertex vertex,
               const std::function<void(Vertex)> &callback, int &time,
               DFSTree &tree) {
  callback(vertex);

  tree[vertex].status = VertexStatus::WAITING;
  tree[vertex].discovery_time = ++time;

  for (Edge out_edge : graph.out_edges(vertex)) {
    DefaultIdType adjacent = out_edge.v;

    if (tree[adjacent].status == VertexStatus::READY) {
      tree[adjacent].parent = vertex;
      visit_rec(graph, Vertex{adjacent}, callback, time, tree);
    }
  }

  tree[vertex].status = VertexStatus::PROCESSED;
  tree[vertex].finishing_time = ++time;
}

} // namespace graphxx::algorithms::dfs

#endif