#pragma once
#include "g_base.hpp"

#include <unordered_map>

namespace graph {

template <GraphType T> class AdjacencyMatrixGraph {
protected:
  using AdjacencyMap = std::unordered_map<Id, Id>;
  using AdjacencyMatrix = std::unordered_map<Id, AdjacencyMap>;
  using EdgeMap = std::unordered_map<Id, Edge>;

public:
  Vertex add_vertex();
  Edge add_edge(const Vertex &, const Vertex &);
  bool remove_vertex(const Vertex &);
  bool remove_edge(const Edge &);

  decltype(auto) vertices();
  decltype(auto) edges();
  decltype(auto) out_edges(const Vertex &);
  decltype(auto) in_edges(const Vertex &);

private:
  AdjacencyMatrix _adj;
  EdgeMap _edge_map;

  Id _next_vid = 0;
  Id _next_eid = 0;
};
} // namespace graph

#include "g_matrix_graph.i.hpp"