#pragma once
#include "g_base.hpp"

#include <list>
#include <unordered_map>
#include <vector>

namespace graph {

template <GraphType T> class AdjacencyListGraph {
protected:
  using EdgeList = std::list<Id>;
  using AdjacencyList = std::unordered_map<Id, EdgeList>;
  using EdgeMap = std::unordered_map<Id, Edge>;

public:
  Vertex add_vertex();
  bool remove_vertex(const Vertex &v);

  Edge add_edge(const Vertex &u, const Vertex &v);
  bool remove_edge(const Edge &e);

  decltype(auto) vertices();
  decltype(auto) edges();
  decltype(auto) out_edges(const Vertex &v);
  decltype(auto) in_edges(const Vertex &v);

private:
  AdjacencyList _adj;
  EdgeMap _edge_map;

  Id _next_vid = 0;
  Id _next_eid = 0;
};
} // namespace graph

#include "g_list_graph.i.hpp"