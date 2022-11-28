#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <unordered_map>

namespace graph::algorithms {

template <concepts::Graph G> class BFS {
public:
  struct BFSVertex {
    VertexStatus status;
    int distance = -1;
    int parent = -1;
  };

  using BFSTree = std::unordered_map<Id, BFSVertex>;

  BFS(G &g);

  BFSTree visit(Vertex source);
  BFSTree visit(Vertex source, std::function<void(Vertex)> f);

private:
  void init();

  G &_graph;
  BFSTree _bfs_tree;
};

} // namespace graph::algorithms

#include "algorithms/bfs.i.hpp"