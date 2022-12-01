#include "algorithms/johnson.hpp"
#include "bellman_ford.hpp"
#include "dijkstra.hpp"

namespace graph::algorithms::johnson {

template <concepts::Graph G, concepts::Subscriptable<Id> C,
          concepts::Numeric WeightType>
Map<WeightType> visit(G &graph, C &&edges_weights) {
  Map<WeightType> map;

  // Add new vertex
  auto q = graph.add_vertex();

  // Add new edge from q to every other vertex
  for (auto vertex : graph.vertices()) {
    auto added_edge = graph.add_edge(q, vertex);
    edges_weights[added_edge] = 0;
  }

  // Run Bellman–Ford algorithm
  auto bf_tree = bellman_ford::visit(graph, q, edges_weights);

  // Reweigh the edges using the values computed by Bellman–Ford algorithm:
  // w(u,v) = w(u,v) + h(u) - h(v)
  for (auto edge : graph.edges()) {
    edges_weights[edge] = edges_weights[edge] + bf_tree[edge.u].distance -
                          bf_tree[edge.v].distance;
  }

  // Remove vertex q
  graph.remove_vertex(q);

  // Run Dijkstra for every vertex
  for (auto vertex_source : graph.vertices()) {
    auto d_tree = dijkstra::visit(graph, vertex_source, edges_weights);
    for (auto vertex_target : graph.vertices()) {
      map[vertex_source][vertex_target].parent = d_tree[vertex_target].parent;
      map[vertex_source][vertex_target].distance = d_tree[vertex_target].distance +
                                          bf_tree[vertex_target].distance -
                                          bf_tree[vertex_source].distance;
    }
  }

  return map;
}
} // namespace graph::algorithms::johnson