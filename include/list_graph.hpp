#pragma once
#include "base.hpp"
#include "id_manager.hpp"
#include "graph_concepts.hpp"

#include <list>
#include <unordered_map>
#include <vector>

namespace graph {

template <concepts::Orientable GraphType> class AdjacencyListGraph {
protected:
  using EdgeList = std::list<Id>;
  using AdjacencyList = std::unordered_map<Id, EdgeList>;
  using EdgeMap = std::unordered_map<Id, Edge>;

public:
  using Tag = GraphType;
  
  AdjacencyListGraph();

  Vertex add_vertex();
  void remove_vertex(const Vertex &v);

  Edge add_edge(const Vertex &u, const Vertex &v);
  void remove_edge(const Edge &e);

  auto vertices() const;
  auto edges() const;
  auto out_edges(const Vertex &v) const;
  auto in_edges(const Vertex &v) const;

private:
  AdjacencyList _adj;
  EdgeMap _edge_map;

  utils::IdManager _vertex_id_manager;
  utils::IdManager _edge_id_manager;
};

} // namespace graph

#include "list_graph.i.hpp"