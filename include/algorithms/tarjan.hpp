#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"
#include <vector>

namespace graph::algorithms::tarjan {

using SCCVector = std::vector<std::vector<Id>>;

template <concepts::Graph G>
SCCVector visit(const G &graph);

} // namespace graph::algorithms::tarjan

#include "algorithms/tarjan.i.hpp"