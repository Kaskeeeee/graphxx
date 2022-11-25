#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"

#include <unordered_map>

namespace graph::algorithms {

template <concepts::Graph G, concepts::Numeric WeightType> class Dijkstra {
  struct DijkstraNode {
    WeightType distance;
    Id previous_hop;
  };

  using DijkstraTree = std::unordered_map<Id, DijkstraNode>;

public:
  Dijkstra(G &graph);

  template <concepts::Subscriptable<Id, WeightType> C>
  DijkstraTree visit(const Vertex &source, C &&weights);

private:
  G &_graph;
  DijkstraTree _tree;
};
} // namespace graph::algorithms

#include "algorithms/dijkstra.i.hpp"