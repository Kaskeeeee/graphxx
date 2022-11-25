#include "base.hpp"
#include "dijkstra.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"
#include "utils.hpp"

#include <queue>
#include <unordered_map>
#include <vector>

namespace graph::algorithms {
template <concepts::Graph G, concepts::Numeric WeightType>
Dijkstra<G, WeightType>::Dijkstra(G &graph) : _graph{graph} {};

template <concepts::Graph G, concepts::Numeric WeightType>
template <concepts::Subscriptable<Id, WeightType> C>
Dijkstra<G, WeightType>::DijkstraTree
Dijkstra<G, WeightType>::visit(const Vertex &source, C &&weights) {
  _tree.clear();
  auto distance_upperbound = std::numeric_limits<WeightType>::max();

  using VertexDistancePair = std::pair<Vertex, WeightType>;
  constexpr auto comparator = [&](const VertexDistancePair &p,
                                  const VertexDistancePair &q) {
    return p.second > q.second;
  };

  std::priority_queue<VertexDistancePair, std::vector<VertexDistancePair>,
                      decltype(comparator)>
      queue{comparator};

  _tree[source] = {.distance = 0, .previous_hop = INVALID_VERTEX};
  queue.push(std::make_pair(source, 0));

  for (auto vertex : _graph.vertices()) {
    if (vertex != source) {
      _tree[vertex] = DijkstraNode{.distance = distance_upperbound,
                                   .previous_hop = INVALID_VERTEX};
      queue.push(std::make_pair(vertex, distance_upperbound));
    }
  }

  // in this implementation, queue keeps also useless elements
  // this is due to the fact that you can't update order in the queue
  // without pushing a new element
  while (!queue.empty()) {
    // extract node and distance of min distance node
    VertexDistancePair top_pair = queue.top();

    // if the distance is not the one up to date, keep on popping elements
    while (_tree[top_pair.first].distance != top_pair.second) {
      queue.pop();
      top_pair = queue.top();
    }
    queue.pop();

    auto [u, u_distance] = top_pair;

    for (auto edge : _graph.out_edges(u)) {
      if (weights[edge] < 0) {
        throw exceptions::InvariantViolationException(
            "negative edge weight found");
      }

      auto alternative_distance = u_distance + weights[edge];
      if (!sum_will_overflow(u_distance, weights[edge]) &&
          alternative_distance < _tree[edge.v].distance) {
        _tree[edge.v].distance = alternative_distance;
        _tree[edge.v].previous_hop = u;
        queue.push(std::make_pair(edge.v, alternative_distance));
      }
    }
  }

  return _tree;
}

} // namespace graph::algorithms