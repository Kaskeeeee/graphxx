#include "algorithms/bellman_ford.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include <cassert>
#include <limits>

namespace graph::algorithms {

template <concepts::Graph G, concepts::Numeric WeightType>
BellmanFord<G, WeightType>::BellmanFord(G &g) : _graph{g} {};

template <concepts::Graph G, concepts::Numeric WeightType>
template <concepts::Subscriptable<Id, WeightType> C>
BellmanFord<G, WeightType>::BellmanFordTree
BellmanFord<G, WeightType>::visit(const Vertex &v, C &&edges_weights) {

  _bf_tree.clear();
  constexpr auto distance_upperbound = std::numeric_limits<WeightType>::max();

  // Initialize verteces distances
  for (auto vertex : _graph.vertices()) {
    _bf_tree[vertex] = {.distance = distance_upperbound,
                        .previous_hop = INVALID_VERTEX};
  }
  _bf_tree[v] = {.distance = 0, .previous_hop = INVALID_VERTEX};

  // Relax edges |nodes| - 1 times
  for (size_t i = 0; i < _graph.vertices().size() - 1; ++i) {
    for (auto edge : _graph.edges()) {
      auto &source = _bf_tree[edge.u];
      auto &target = _bf_tree[edge.v];

      if (!sum_will_overflow(source.distance, edges_weights[edge]) &&
          source.distance + edges_weights[edge] < target.distance) {
        target.distance = source.distance + edges_weights[edge];
        target.previous_hop = edge.u;
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (auto edge : _graph.edges()) {
    auto &source = _bf_tree[edge.u];
    auto &target = _bf_tree[edge.v];

    if (!sum_will_overflow(source.distance, edges_weights[edge]) &&
        source.distance + edges_weights[edge] < target.distance) {
      throw exceptions::InvariantViolationException("negative cycle found");
    }
  }

  return _bf_tree;
}
} // namespace graph::algorithms