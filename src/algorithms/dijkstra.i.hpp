#include "base.hpp"
#include "dijkstra.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace graph::algorithms::dijkstra {

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights) {
  Tree<WeightType> tree;
  auto distance_upperbound = std::numeric_limits<WeightType>::max();

  using VertexDistancePair = std::pair<Vertex, WeightType>;
  constexpr auto COMPARATOR = [&](const VertexDistancePair &p,
                                  const VertexDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexDistancePair, std::vector<VertexDistancePair>,
                      decltype(COMPARATOR)>
      queue{COMPARATOR};

  tree[source] = {.distance = 0, .parent = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : graph.vertices()) {
    if (vertex != source) {
      tree[vertex] =
          Node{.distance = distance_upperbound, .parent = INVALID_VERTEX};
    }
  }

  while (!queue.empty()) {
    auto u = queue.top().first;
    queue.pop();

    for (auto edge : graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = tree[u].distance + weights[edge];
      if (!sum_will_overflow(tree[u].distance, weights[edge]) &&
          alternative_distance < tree[edge.v].distance) {
        tree[edge.v].distance = alternative_distance;
        tree[edge.v].parent = u;
        queue.push(std::make_pair(edge.v, alternative_distance));
      }
    }
  }

  return tree;
}

} // namespace graph::algorithms::dijkstra