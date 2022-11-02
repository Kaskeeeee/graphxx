#pragma once

#include "g_concepts.hpp"
#include <functional>

namespace graph {
namespace algorithms {
enum class VertexStatus {
  READY,
  WAITING,
  PROCESSED,
};

struct BFSVertex {
  VertexStatus status;
  int distance = -1;
  int parent = -1;
};

template <concepts::Graph G> class BFS {
  using BFSTree = std::unordered_map<Id, BFSVertex>;

public:
  BFS(G &g);

  BFSTree visit(Vertex source);
  BFSTree visit(Vertex source, std::function<void(Vertex)> f);

private:
  void init();

  G &_graph;
  BFSTree _bfs_tree;
};

} // namespace algorithms
} // namespace graph