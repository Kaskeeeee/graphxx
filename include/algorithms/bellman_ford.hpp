#pragma once

#include "algorithms_base.hpp"
#include "graph_concepts.hpp"
#include <unordered_map>

namespace graph::algorithms {

template <concepts::Graph G, concepts::Numeric WeightType> class BellmanFord {
  struct BellmanFordNode {
    WeightType distance;
    Id previous_hop;
  };
  using BellmanFordTree = std::unordered_map<Id, BellmanFordNode>;

public:
  BellmanFord(G &g);

  template <concepts::Subscriptable<Id, WeightType> C>
  BellmanFordTree visit(const Vertex &v, C &&edges_weights);

private:
  G &_graph;
  BellmanFordTree _bf_tree;
};

} // namespace graph::algorithms

#include "algorithms/bellman_ford.i.hpp"