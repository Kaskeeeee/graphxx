#include "algorithms/bellman_ford.hpp"
#include "exceptions.hpp"
#include <cassert>
#include <limits>

namespace graph::algorithms::bellman_ford {

template <concepts::Numeric WeightType>
Node<WeightType>::Node()
    : distance{std::numeric_limits<WeightType>::max()}, parent{
                                                            INVALID_VERTEX} {};

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Tree<WeightType> visit(const G &graph, const Vertex &v, C &&edges_weights) {
  Tree<WeightType> tree;
  tree[v].distance = 0;
  tree[v].parent = INVALID_VERTEX;

  // Relax edges |nodes| - 1 times
  for (size_t i = 0; i < graph.vertices().size() - 1; ++i) {
    for (auto edge : graph.edges()) {
      auto &source = tree[edge.u];
      auto &target = tree[edge.v];

      if (!sum_will_overflow(source.distance, edges_weights[edge]) &&
          source.distance + edges_weights[edge] < target.distance) {
        target.distance = source.distance + edges_weights[edge];
        target.parent = edge.u;
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (auto edge : graph.edges()) {
    auto &source = tree[edge.u];
    auto &target = tree[edge.v];

    if (!sum_will_overflow(source.distance, edges_weights[edge]) &&
        source.distance + edges_weights[edge] < target.distance) {
      throw exceptions::InvariantViolationException("negative cycle found");
    }
  }

  return tree;
}
} // namespace graph::algorithms::bellman_ford