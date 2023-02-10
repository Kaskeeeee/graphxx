#pragma once

#include "base.hpp"           // Vertex
#include "graph_concepts.hpp" // concepts::Graph

#include <numeric> // std::iota
#include <vector>  // std::vector

namespace graphxx {

template <concepts::Graph G>
std::vector<Vertex<G>> get_sorted_vertices(const G &graph) {
  std::vector<Vertex<G>> vertices(graph.num_vertices());
  std::iota(vertices.begin(), vertices.end(), 0);
  return vertices;
}

template <concepts::Graph G>
std::vector<std::pair<Vertex<G>, Vertex<G>>> get_sorted_edges(const G &graph) {
  std::vector<std::pair<Vertex<G>, Vertex<G>>> edges;

  for (auto &out_edges : graph) {
    for (auto &edge : out_edges) {
      edges.emplace_back(graph.get_source(edge), graph.get_target(edge));
    }
  }

  std::sort(edges.begin(), edges.end());
  return edges;
}

} // namespace graphxx