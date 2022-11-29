#include "a_star.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace graph::algorithms::a_star {

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &source, C &&weights,
                       C &&heuristic_weights) {
  Tree<WeightType> tree;
  auto distance_upperbound = std::numeric_limits<WeightType>::max();

  using VertexHeuristicDistancePair = std::pair<Vertex, WeightType>;
  constexpr auto comparator = [&](const VertexHeuristicDistancePair &p,
                                  const VertexHeuristicDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexHeuristicDistancePair,
                      std::vector<VertexHeuristicDistancePair>,
                      decltype(comparator)>
      queue{comparator};

  tree[source] = {.distance = 0,
                  .heuristic_distance = heuristic_weights[source],
                  .parent = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : graph.vertices()) {
    if (vertex != source) {
      tree[vertex] = Node{.distance = distance_upperbound,
                          .heuristic_distance = heuristic_weights[vertex],
                          .parent = INVALID_VERTEX};
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
      if (tree[next_pair.first].heuristic_distance == next_pair.second) {
        break;
      }

      queue.pop();
      next_pair = queue.top();
    }

    queue.pop();

    [[unlikely]] if (tree[next_pair.first].heuristic_distance !=
                     next_pair.second) {
      return tree;
    }

    auto [u, _] = next_pair;

    for (auto edge : graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = tree[u].distance + weights[edge];
      auto new_heuristic_distance =
          alternative_distance + heuristic_weights[edge.v];
      if (!sum_will_overflow(tree[u].distance, weights[edge]) &&
          !sum_will_overflow(alternative_distance, heuristic_weights[edge.v]) &&
          alternative_distance < tree[edge.v].distance) {
        tree[edge.v].distance = alternative_distance;
        tree[edge.v].parent = u;
        tree[edge.v].heuristic_distance = new_heuristic_distance;
        queue.push(std::make_pair(edge.v, new_heuristic_distance));
      }
    }
  }
  return tree;
}
} // namespace graph::algorithms::a_star