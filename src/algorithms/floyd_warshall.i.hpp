#include "algorithms/floyd_warshall.hpp"
#include "utils.hpp"

#include <limits>

namespace graph::algorithms::floyd_warshall {

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType = DecaySubscriptValue<Id, C>>
Map<WeightType> visit(G &graph, C &&weights) {
  Map<WeightType> map;
  for (auto [id, u, v] : graph.edges()) {
    map[u][v] = {.distance = weights[id], parent : u};
  }

  for (auto u : graph.vertices()) {
    for (auto v : graph.vertices()) {
      for (auto w : graph.vertices()) {
        auto &v_to_u = map[v][u];
        auto &u_to_w = map[u][w];
        auto &v_to_w = map[v][w];
        if (!sum_will_overflow(v_to_u.distance, u_to_w.distance) &&
            v_to_u.distance + u_to_w.distance < v_to_w.distance) {
          v_to_w.distance = v_to_u.distance + u_to_w.distance;
          v_to_w.previous_hop = u_to_w.previous_hop;
        }
      }
    }
  }

  return map;
}

} // namespace graph::algorithms::floyd_warshall