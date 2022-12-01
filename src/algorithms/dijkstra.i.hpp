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
  constexpr auto comparator = [&](const VertexDistancePair &p,
                                  const VertexDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexDistancePair, std::vector<VertexDistancePair>,
                      decltype(comparator)>
      queue{comparator};

  tree[source] = {.distance = 0, .parent = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : graph.vertices()) {
    if (vertex != source) {
      tree[vertex] =
          Node{.distance = distance_upperbound, .parent = INVALID_VERTEX};
      queue.push(std::make_pair(vertex, distance_upperbound));
    }
  }

  // in this implementation, queue keeps also useless elements
  // this is due to the fact that you can't update order in the queue
  // without pushing a new element
  while (!queue.empty()) {
    // extract node and distance of min distance node
    auto next_pair = queue.top();

    // if the distance is not the one up to date, keep on popping elements
    // WARNING: to avoid undefined behavior we pop from the queue only if
    // it still has elements inside, otherwise there may be an infinite loop
    for (size_t i = 0; i < queue.size() - 1; ++i) {
      if (tree[next_pair.first].distance == next_pair.second) {
        break;
      }

      queue.pop();
      next_pair = queue.top();
    }

    queue.pop();

    [[unlikely]] if (tree[next_pair.first].distance != next_pair.second) {
      return tree;
    }

    auto [u, u_distance] = next_pair;

    for (auto edge : graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = u_distance + weights[edge];
      if (!sum_will_overflow(u_distance, weights[edge]) &&
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