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

#include "algorithms/ford_fulkerson.hpp"
#include "algorithms_base.hpp"
#include "base.hpp"
#include "exceptions.hpp"

#include <limits>
#include <queue>

namespace graphxx::algorithms::ford_fulkerson {

template <concepts::Graph G, concepts::Graph DG, std::invocable<Edge<G>> Weight,
          concepts::Numeric Distance>
DistanceTree<G, Distance> bfs(const G &graph, const DG &digraph,
                              const Vertex<G> &source, Weight edge_capacity,
                              const FlowMap<Edge<G>, Distance> &edges_flow) {

  DistanceTree<G, Distance> distance_tree;
  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); ++vertex) {
    distance_tree.push_back(Node{.status = VertexStatus::READY,
                                 .edge = nullptr,
                                 .residual_capacity = -1,
                                 .parent = INVALID_VERTEX<G>});
  }

  distance_tree[source].status = VertexStatus::WAITING;
  distance_tree[source].parent = source;

  std::queue<Vertex<G>> queue;
  queue.push(source);

  while (!queue.empty()) {
    Vertex<G> vertex_id = queue.front();
    queue.pop();

    for (auto out_edge : graph[vertex_id]) {
      if (edge_capacity(out_edge) < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      Vertex<G> adjacent = graph.get_target(out_edge);

      Distance cf;

      if (digraph.get_source(out_edge) != INVALID_VERTEX<G> &&
          digraph.get_target(out_edge) != INVALID_VERTEX<G>) {
        cf = edge_capacity(out_edge) - edges_flow.at(out_edge);
      } else {
        cf = edge_capacity(out_edge) + edges_flow.at(out_edge);
      }

      if (distance_tree[adjacent].status == VertexStatus::READY && cf > 0) {
        distance_tree[adjacent].status = VertexStatus::WAITING;
        distance_tree[adjacent].parent = vertex_id;
        distance_tree[adjacent].edge = out_edge;
        distance_tree[adjacent].residual_capacity = cf;
        queue.push(adjacent);
      }
    }

    distance_tree[vertex_id].status = VertexStatus::PROCESSED;
  }

  return distance_tree;
}

template <concepts::Graph G, std::invocable<Edge<G>> Weight, typename Distance>
FFpair<G, Distance> visit(const G &graph, Vertex<G> &source, Vertex<G> &sink,
                          Weight weight) {

  FlowMap<G, Distance> flow;

  AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED> g{graph};

  for (Vertex<G> vertex = 0; vertex < graph.num_vertices(); vertex++) {
    auto out_edge_list = graph[vertex];
    for (auto edge : out_edge_list) {
      flow[edge] = 0;
    }
  }

  Distance max_flow = 0;

  auto tree = bfs(g, graph, source, weight, flow);

  // Relax edges |nodes| - 1 times
  while (tree[sink].status == VertexStatus::PROCESSED) {
    Distance path_flow = std::numeric_limits<Distance>::max();
    for (Vertex<G> v = sink; v != source; v = tree[v].parent) {
      path_flow = std::min(path_flow, tree[v].residual_capacity);
    }

    for (Vertex<G> v = sink; v != source; v = tree[v].parent) {
      if (graph.get_source(tree[v].edge) != INVALID_VERTEX<G> &&
          graph.get_target(tree[v].edge) != INVALID_VERTEX<G>) {
        flow[tree[v].edge] += path_flow;
      } else {
        flow[tree[v].edge] -= path_flow;
      }
    }

    // Adding the path flows
    max_flow += path_flow;

    tree = bfs(g, graph, source, weight, flow);
  }

  return FFpair{flow, max_flow};
}

} // namespace graphxx::algorithms::ford_fulkerson