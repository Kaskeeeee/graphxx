#pragma once
#include "base.hpp"
#include "graph_concepts.hpp"
#include <unordered_map>
#include <vector>

namespace graph::algorithms {

template <concepts::Graph G, concepts::Numeric WeightType> class Kruskal {

  // Vector of edge Ids
  using KruskalTree = std::vector<Edge>;

  // Map that divides the vertices into "clusters" and allows us to check if two
  // vertices belong to the same cluster or not and hence decide whether adding
  // an edge creates a cycle
  using ClusterMap = std::unordered_map<Id, int>;

public:
  Kruskal(G &g);
  KruskalTree visit(std::unordered_map<Id, WeightType> &edges_weights);

  // Method that finds the representative vertex of the cluster
  Id find_representative(Id id);

private:
  G &_graph;
  KruskalTree _k_tree;
  ClusterMap _clusters;
};

} // namespace graph::algorithms

#include "algorithms/kruskal.i.hpp"