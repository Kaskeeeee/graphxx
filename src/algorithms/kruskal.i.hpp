#include "algorithms/kruskal.hpp"
#include <queue>

namespace graph::algorithms::kruskal {

// Search for the repserentative vertex of the cluster, which is that element in
// the map with same key and value
Id find_representative(ClusterMap &map, Id id) {
  if (id == map[id]) {
    return id;
  } else {
    return find_representative(map, map[id]);
  }
};

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Tree visit(const G &graph, C &&weights) {

  Tree tree;
  ClusterMap map;

  using EdgeWeightPair = std::pair<Edge, WeightType>;
  constexpr auto comparator = [&](const EdgeWeightPair &p,
                                  const EdgeWeightPair &q) {
    return p.second > q.second;
  };

  std::priority_queue<EdgeWeightPair, std::vector<EdgeWeightPair>,
                      decltype(comparator)>
      queue{comparator};

  // Queue ordered by ascending edge weight
  for (auto edge : weights) {
    queue.push(std::make_pair(edge.first, edge.second));
  }

  // Initialize every vertex cluster with the vertex Id itself
  for (auto vertex : _graph.vertices()) {
    map[vertex] = vertex.id;
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
      tree.push_back(edge);
      map[edge.u] = map[edge.v];
    }
  }

  return tree;
};

} // namespace graph::algorithms::kruskal