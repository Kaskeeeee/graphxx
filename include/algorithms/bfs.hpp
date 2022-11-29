#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include <functional>
#include <unordered_map>

namespace graph::algorithms::bfs {
struct BFSVertex {
  VertexStatus status;
  int distance = -1;
  int parent = -1;
};

using BFSTree = std::unordered_map<Id, BFSVertex>;

template <concepts::Graph G> BFSTree visit(const G &graph, Vertex source);

template <concepts::Graph G>
BFSTree visit(const G &graph, Vertex source, std::function<void(Vertex)> f);
} // namespace graph::algorithms::bfs

#include "algorithms/bfs.i.hpp"