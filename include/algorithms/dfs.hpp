#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <unordered_map>

namespace graph::algorithms {

template <concepts::Graph G> class DFS {
  struct DFSVertex {
    VertexStatus status;
    int parent = -1;
    int discovery_time = -1;
    int finishing_time = -1;
  };

  using DFSForest = std::unordered_map<Id, DFSVertex>;

public:
  DFS(G &g);

  DFSForest visit();
  DFSForest visit(std::function<void(Vertex)> f);

private:
  void init();
  void visit_rec(Vertex vertex, std::function<void(Vertex)> f);

  G &_graph;
  DFSForest _dfs_forest;
  int _time = 0;
};

} // namespace graph::algorithms

#include "algorithms/dfs.i.hpp"