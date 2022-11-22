#include "algorithms/bellman_ford.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include <cassert>
#include <limits>

namespace graph::algorithms {

template <concepts::Graph G, concepts::IsNumberLike WeightType>
BellmanFord<G, WeightType>::BellmanFord(G &g) : _graph{g} {};

template <concepts::Graph G, concepts::IsNumberLike WeightType>
BellmanFord<G, WeightType>::BellmanFordTree BellmanFord<G, WeightType>::visit(
    const Vertex &v, const std::unordered_map<Id, WeightType> &edges_weights) {

  const auto MAX_WEIGHT = std::numeric_limits<WeightType>::max();

  // Initialize verteces distances
  for (auto vertex : _graph.vertices()) {
    _bf_tree[vertex] = {.distance = MAX_WEIGHT, .previous_hop = INVALID_VERTEX};
  }
  _bf_tree[v] = {.distance = 0, .previous_hop = v};

  // Relax edges |nodes| - 1 times
  for (int i = 0; i < _graph.vertices().size() - 1; i++) {
    for (auto edge : _graph.edges()) {
      auto &source = _bf_tree[edge.u];
      auto &target = _bf_tree[edge.v];

      if (source.distance != MAX_WEIGHT &&
          !sum_will_overflow(source.distance, edges_weights[edge]) &&
          source.distance + edges_weights[edge] < target.distance) {
        target.distance = source.distance + edges_weights[edge];
        target.previous = edge.u;
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (auto edge : _graph.edges()) {
    auto &source = _bf_tree[edge.u];
    auto &target = _bf_tree[edge.v];

    if (source.distance != MAX_WEIGHT &&
        !sum_will_overflow(source.distance, edges_weights[edge]) &&
        source.distance + edges_weights[edge] < target.distance) {
      throw exception::NegativeCycleException();
    }
  }

  return _bf_tree;
}
} // namespace graph::algorithms