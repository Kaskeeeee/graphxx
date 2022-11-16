#include "algorithms/algorithms_base.hpp"
#include "algorithms/dfs.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <queue>

namespace graph {

namespace algorithms {
template <concepts::Graph G> DFS<G>::DFS(G &g) : _graph{g} {};

template <concepts::Graph G> void DFS<G>::init() {
  _time = 0;
  _dfs_forest.clear();
  for (Vertex vertex : _graph.vertices()) {
    _dfs_forest[vertex] = DFSVertex{VertexStatus::READY};
  }
}

template <concepts::Graph G> DFS<G>::DFSForest DFS<G>::visit() {
  return visit([](Vertex v) {});
}

template <concepts::Graph G>
DFS<G>::DFSForest DFS<G>::visit(std::function<void(Vertex)> f) {
  init();

  for (Vertex vertex : _graph.vertices()) {
    if (_dfs_forest[vertex].status == VertexStatus::READY) {
      visit_rec(vertex);
    }
  }

  return _dfs_forest;
}

template <concepts::Graph G>
void DFS<G>::visit_rec(Vertex vertex, std::function<void(Vertex)> f) {
  f(vertex);

  _dfs_forest[vertex].status = VertexStatus::WAITING;
  _dfs_forest[vertex].discovery_time = ++_time;

  for (Edge out_edge : _graph.out_edges(vertex)) {
    Id adjacent = out_edge.v;

    if (_dfs_forest[adjacent].status == VertexStatus::READY) {
      _dfs_forest[adjacent].parent = vertex;
      visit_rec({adjacent});
    }
  }

  _dfs_forest[vertex].status = VertexStatus::PROCESSED;
  _dfs_forest[vertex].finishing_time = ++_time;
}

} // namespace algorithms
} // namespace graph