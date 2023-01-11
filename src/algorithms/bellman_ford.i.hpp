#include "algorithms/bellman_ford.hpp"
#include "base.hpp"
#include "exceptions.hpp"
#include "graph_concepts.hpp"

#include <limits>

namespace graph::algorithms::bellman_ford {

template <concepts::Numeric WeightType>
Node<WeightType>::Node()
    : distance{std::numeric_limits<WeightType>::max()}, parent{
                                                            INVALID_VERTEX} {};

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Tree<WeightType> visit(const G &graph, const Vertex &source,
                       C &&edges_weights) {
  Tree<WeightType> tree;
  tree[source].distance = 0;
  tree[source].parent = INVALID_VERTEX;

  // Relax edges |nodes| - 1 times
  for (int64_t i = 0; i < static_cast<int64_t>(graph.vertices().size() - 1);
       ++i) {
    for (auto edge : graph.edges()) {
      auto &edge_source = tree[edge.u];
      auto &edge_target = tree[edge.v];

      if (!sum_will_overflow(edge_source.distance, edges_weights[edge]) &&
          edge_source.distance + edges_weights[edge] < edge_target.distance) {
        edge_target.distance = edge_source.distance + edges_weights[edge];
        edge_target.parent = edge.u;
      }
    }
  }

  // Detect if there are negative cycles
  // if value changes then we have a negative cycle in the graph and we cannot
  // find the shortest distances
  for (auto edge : graph.edges()) {
    auto &edge_source = tree[edge.u];
    auto &edge_target = tree[edge.v];

    if (!sum_will_overflow(edge_source.distance, edges_weights[edge]) &&
        edge_source.distance + edges_weights[edge] < edge_target.distance) {
      throw exceptions::InvariantViolationException("negative cycle found");
    }
  }

  return tree;
}
} // namespace graph::algorithms::bellman_ford