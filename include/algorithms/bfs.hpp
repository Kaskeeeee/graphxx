#pragma once

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
BFSTree visit(const G &graph, Vertex source, const std::function<void(Vertex)> &callback);
} // namespace graph::algorithms::bfs

#include "algorithms/bfs.i.hpp"