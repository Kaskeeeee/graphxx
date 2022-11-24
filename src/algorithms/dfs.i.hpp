#include "algorithms/algorithms_base.hpp"
#include "algorithms/dfs.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <queue>

namespace graph::algorithms {

template <concepts::Graph G> DFS<G>::DFS(G &g) : _graph{g} {};

template <concepts::Graph G> void DFS<G>::init() {
  _time = 0;
  _dfs_tree.clear();
  for (Vertex vertex : _graph.vertices()) {
    _dfs_tree[vertex] = DFSVertex{VertexStatus::READY};
  }
}

template <concepts::Graph G> DFS<G>::DFSTree DFS<G>::visit(Vertex source) {
  return visit(Vertex source, [](Vertex v) {});
}

template <concepts::Graph G>
DFS<G>::DFSTree DFS<G>::visit(Vertex source, std::function<void(Vertex)> f) {
  init();

  visit_rec(source);

  return _dfs_tree;
}

template <concepts::Graph G>
void DFS<G>::visit_rec(Vertex vertex, std::function<void(Vertex)> f) {
  f(vertex);

  _dfs_tree[vertex].status = VertexStatus::WAITING;
  _dfs_tree[vertex].discovery_time = ++_time;

  for (Edge out_edge : _graph.out_edges(vertex)) {
    Id adjacent = out_edge.v;

    if (_dfs_tree[adjacent].status == VertexStatus::READY) {
      _dfs_tree[adjacent].parent = vertex;
      visit_rec({adjacent});
    }
  }

  _dfs_tree[vertex].status = VertexStatus::PROCESSED;
  _dfs_tree[vertex].finishing_time = ++_time;
}

} // namespace graph::algorithms