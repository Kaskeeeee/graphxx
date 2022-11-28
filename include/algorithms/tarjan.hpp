#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include <unordered_map>
#include <vector>

namespace graph::algorithms {

template <concepts::Graph G> class Tarjan {
  struct TarjanVertex {
    int index = -1;
    int lowlink = -1;
    bool onStack = false;
  };

  using SCCVector = std::vector<std::vector<Id>>;
  using StackVector = std::vector<Id>;
  using TarjanTree = std::unordered_map<Id, TarjanVertex>;

public:
  Tarjan(G &g);

  SCCVector visit();

private:
  void init();
  void tarjan_rec(Vertex vertex);

  G &_graph;
  SCCVector _scc_vector;
  TarjanTree _tarjan_tree;
  StackVector _stack;
  int index;
};

} // namespace graph::algorithms

#include "algorithms/tarjan.i.hpp"