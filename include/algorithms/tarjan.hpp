#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"
#include <vector>

namespace graph::algorithms::tarjan {

struct TarjanVertex {
  int index = -1;
  int low_link = -1;
  bool on_stack = false;
};

using StackVector = std::vector<Id>;
using TarjanTree = std::unordered_map<Id, TarjanVertex>;
using SCCVector = std::vector<StackVector>;

template <concepts::Graph G> SCCVector visit(const G &graph);

} // namespace graph::algorithms::tarjan

#include "algorithms/tarjan.i.hpp"