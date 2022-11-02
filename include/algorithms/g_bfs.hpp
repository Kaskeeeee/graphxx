#pragma once

#include "g_algorithms_base.hpp"
#include "g_concepts.hpp"
#include <functional>
#include <unordered_map>

namespace graph {
namespace algorithms {

template <concepts::Graph G> class BFS {
  struct BFSVertex {
    VertexStatus status;
    int distance = -1;
    int parent = -1;
  };

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

#include "g_bfs.i.hpp"