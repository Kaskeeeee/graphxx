#pragma once

#include "g_algorithms_base.hpp"
#include "g_concepts.hpp"
#include <functional>
#include <unordered_map>

namespace graph {
namespace algorithms {

struct DFSVertex {
  VertexStatus status;
  int parent = -1;
  int discovery_time = -1;
  int finishing_time = -1;
};

template <concepts::Graph G> class DFS {
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

} // namespace algorithms
} // namespace graph

#include "g_dfs.i.hpp"