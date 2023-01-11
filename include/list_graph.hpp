#pragma once

#include "base.hpp"
#include "graph_concepts.hpp"
#include "id_manager.hpp"

#include <array>
#include <list>
#include <unordered_map>

namespace graph {

template <Directedness D> class AdjacencyListGraph {

protected:
  using EdgeList = std::list<Id>;
  using AdjacencyList = std::unordered_map<Id, EdgeList>;
  using EdgeWrapper =
      std::conditional_t<D == Directedness::DIRECTED, std::array<Edge, 1>,
                         std::array<Edge, 2>>;
  using EdgeMap = std::unordered_map<Id, EdgeWrapper>;

public:
  static constexpr Directedness DIRECTEDNESS = D;

  AdjacencyListGraph();

  template <Directedness DN>
  explicit AdjacencyListGraph(const AdjacencyListGraph<DN> &graph);

  Vertex add_vertex();
  void remove_vertex(const Vertex &v);

  Edge add_edge(const Vertex &u, const Vertex &v);
  void remove_edge(const Edge &e);

  auto vertices() const;
  auto edges() const;
  auto out_edges(const Vertex &v) const;
  auto in_edges(const Vertex &v) const;

  Vertex get_vertex(const Id &id) const;
  Edge get_edge(const Id &id) const;

private:
  AdjacencyList _adj;
  EdgeMap _edge_map;

  utils::IdManager _vertex_id_manager;
  utils::IdManager _edge_id_manager;
};

} // namespace graph

#include "list_graph.i.hpp"