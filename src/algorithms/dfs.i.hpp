#include "algorithms_base.hpp"
#include "algorithms/dfs.hpp"
#include "graph_concepts.hpp"

#include <functional>

namespace graph::algorithms::dfs {

template <concepts::Graph G> DFSTree visit(const G &graph, Vertex source) {
  return visit(graph, source, [](Vertex) {});
}

template <concepts::Graph G>
DFSTree visit(const G &graph, Vertex source, const std::function<void(Vertex)> &callback) {
  DFSTree tree;
  int time = 0;
  for (Vertex vertex : graph.vertices()) {
    tree[vertex] = DFSVertex{VertexStatus::READY};
  }

  visit_rec(graph, source, callback, time, tree);

  return tree;
}

template <concepts::Graph G>
void visit_rec(const G &graph, Vertex vertex, const std::function<void(Vertex)> &callback,
               int &time, DFSTree &tree) {
  callback(vertex);

  tree[vertex].status = VertexStatus::WAITING;
  tree[vertex].discovery_time = ++time;

  for (Edge out_edge : graph.out_edges(vertex)) {
    Id adjacent = out_edge.v;

    if (tree[adjacent].status == VertexStatus::READY) {
      tree[adjacent].parent = vertex;
      visit_rec(graph, Vertex{adjacent}, callback, time, tree);
    }
  }

  tree[vertex].status = VertexStatus::PROCESSED;
  tree[vertex].finishing_time = ++time;
}

} // namespace graph::algorithms::dfs