#include "algorithms/floyd_warshall.hpp"
#include "utils.hpp"

#include <limits>

namespace graph::algorithms::floyd_warshall {

template <concepts::Numeric WeightType>
Node<WeightType>::Node()
    : distance{std::numeric_limits<WeightType>::max()}, parent{
                                                            INVALID_VERTEX} {};

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Map<WeightType> visit(G &graph, C &&weights) {
  Map<WeightType> map;
  for (auto edge : graph.edges()) {
    map[edge.u][edge.v].distance = weights[edge.id];
    map[edge.u][edge.v].parent = edge.u;
  }

  for (auto u : graph.vertices()) {
    for (auto v : graph.vertices()) {
      for (auto w : graph.vertices()) {
        if (v == w) {
          map[v][w].distance = 0;
          map[v][w].parent = INVALID_VERTEX;
          continue;
        }
        auto &v_to_u = map[v][u];
        auto &u_to_w = map[u][w];
        auto &v_to_w = map[v][w];
        if (!sum_will_overflow(v_to_u.distance, u_to_w.distance) &&
            v_to_u.distance + u_to_w.distance < v_to_w.distance) {
          v_to_w.distance = v_to_u.distance + u_to_w.distance;
          v_to_w.parent = u_to_w.parent;
        }
      }
    }
  }

  return map;
}

} // namespace graph::algorithms::floyd_warshall