#include "algorithms/algorithms_base.hpp"
#include "algorithms/bfs.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <queue>

namespace graph::algorithms::bfs {

template <concepts::Graph G> BFSTree visit(const G &graph, Vertex source) {
  return visit(graph, source, [](Vertex) {});
}

template <concepts::Graph G>
BFSTree visit(const G &graph, Vertex source, std::function<void(Vertex)> f) {
  BFSTree tree;
  for (auto vertex : graph.vertices()) {
    tree[vertex] = BFSVertex{VertexStatus::READY};
  }

  tree[source].status = VertexStatus::WAITING;
  tree[source].distance = 0;
  tree[source].parent = -1;

  std::queue<Id> queue;
  queue.push(source);

  while (!queue.empty()) {
    Id vertex_id = queue.front();
    queue.pop();

    f(Vertex{vertex_id});

    for (Edge out_edge : graph.out_edges(Vertex{vertex_id})) {
      Id adjacent = out_edge.v;

      if (tree[adjacent].status == VertexStatus::READY) {
        tree[adjacent].status = VertexStatus::WAITING;
        tree[adjacent].distance = tree[vertex_id].distance + 1;
        tree[adjacent].parent = vertex_id;
        queue.push(adjacent);
      }
    }

    tree[vertex_id].status = VertexStatus::PROCESSED;
  }

  return tree;
}

} // namespace graph::algorithms::bfs
