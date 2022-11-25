#include "algorithms/kruskal.hpp"
#include <queue>

namespace graph::algorithms {

template <concepts::Graph G, concepts::Numeric WeightType>
Kruskal<G, WeightType>::Kruskal(G &g) : _graph{g} {};

// Search for the repserentative vertex of the cluster, which is that element in
// the map with same key and value
template <concepts::Graph G, concepts::Numeric WeightType>
Id Kruskal<G, WeightType>::find_representative(Id id) {
  if (id == _clusters[id]) {
    return id;
  } else {
    return find_representative(_clusters[id]);
  }
};

template <concepts::Graph G, concepts::Numeric WeightType>
template <concepts::Subscriptable<Id, WeightType> C>
Kruskal<G, WeightType>::KruskalTree
Kruskal<G, WeightType>::visit(C &&edges_weights) {

  _k_tree.clear();
  _clusters.clear();

  using EdgeWeightPair = std::pair<Edge, WeightType>;
  constexpr auto comparator = [&](const EdgeWeightPair &p,
                                  const EdgeWeightPair &q) {
    return p.second > q.second;
  };

  std::priority_queue<EdgeWeightPair, std::vector<EdgeWeightPair>,
                      decltype(comparator)>
      queue{comparator};

  // Queue ordered by ascending edge weight
  for (auto edge : edges_weights) {
    queue.push(std::make_pair(edge.first, edge.second));
  }

  // Initialize every vertex cluster with the vertex Id itself
  for (auto vertex : _graph.vertices()) {
    _clusters[vertex] = vertex.id;
  }

  // At every iteration, we want to add an edge to the tree, but we must check
  // if adding the edge will create a cycle. To do so, we analyse if the
  // vertices of the edge belong to different clusters. If yes, we can safely
  // add it. Then, we update the representative vertex of the cluster of one of
  // the two verteces
  while (!queue.empty()) {
    EdgeWeightPair top_pair = queue.top();
    queue.pop();

    auto edge = top_pair.first;

    if (find_representative(edge.u) != find_representative(edge.v)) {
      _k_tree.push_back(edge);
      _clusters[edge.u] = _clusters[edge.v];
    }
  }

  return _k_tree;
};

} // namespace graph::algorithms