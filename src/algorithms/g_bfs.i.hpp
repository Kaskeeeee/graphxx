#include "algorithms/g_bfs.hpp"
#include "g_concepts.hpp"
#include <functional>
#include <queue>

namespace graph {

namespace algorithms {

template <concepts::Graph G> BFS<G>::BFS(G &g) : _graph{g} {};

template <concepts::Graph G> void BFS<G>::init() {
  _bfs_tree.clear();
  for (auto vertex : _graph.vertices()) {
    _bfs_tree[vertex.id] = BFSVertex{VertexStatus::READY};
  }
}

template <concepts::Graph G> BFS<G>::BFSTree BFS<G>::visit(Vertex source) {
  return visit(source, [](Vertex v) {});
}

template <concepts::Graph G>
BFS<G>::BFSTree BFS<G>::visit(Vertex source, std::function<void(Vertex)> f) {
  init();

  _bfs_tree[source.id].status = VertexStatus::WAITING;
  _bfs_tree[source.id].distance = 0;
  _bfs_tree[source.id].parent = -1;

  std::queue<Id> queue;
  queue.push(source.id);

  while (!queue.empty()) {
    Id vertex_id = queue.front();
    queue.pop();

    f({vertex_id});

    for (Edge out_edge : _graph.out_edges({vertex_id})) {
      Id adjacent = out_edge.target;

      if (_bfs_tree[adjacent].status == VertexStatus::READY) {
        _bfs_tree[adjacent].status = VertexStatus::WAITING;
        _bfs_tree[adjacent].distance = _bfs_tree[vertex_id].distance + 1;
        _bfs_tree[adjacent].parent = vertex_id;
        queue.push(adjacent);
      }
    }

    _bfs_tree[vertex_id].status = VertexStatus::PROCESSED;
  }

  return _bfs_tree;
}

} // namespace algorithms
} // namespace graph