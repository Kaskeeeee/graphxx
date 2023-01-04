#pragma once
#include "base.hpp"
#include "exceptions.hpp"
#include "id_manager.hpp"
#include "list_graph.hpp"

#include <cassert>
#include <ranges>
#include <type_traits>

namespace graph {
template <Directedness D>
AdjacencyListGraph<D>::AdjacencyListGraph()
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {};

template <Directedness D>
template <Directedness DN>
AdjacencyListGraph<D>::AdjacencyListGraph(const AdjacencyListGraph<DN> &graph)
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {

  for (Vertex v : graph.vertices()) {
    _adj[v] = {};
  }

  for (Edge e : graph.edges()) {
    add_edge(e.u, e.v);
  }
};

template <Directedness D> Vertex AdjacencyListGraph<D>::add_vertex() {
  auto id = _vertex_id_manager.allocate();

  Vertex v{id};
  _adj[v] = {};
  return v;
};

template <Directedness D>
Edge AdjacencyListGraph<D>::add_edge(const Vertex &u, const Vertex &v) {
  if (!_adj.contains(u) || !_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  auto id = _edge_id_manager.allocate();

  if constexpr (D == Directedness::UNDIRECTED) {
    Edge e1{id, u, v};
    Edge e2{id, v, u};
    _adj[u].push_back(e1);
    _adj[v].push_back(e2);

    EdgeWrapper links = {e1, e2};
    _edge_map.insert({id, links});
    return e1;
  } else {
    Edge e{id, u, v};
    _adj[u].push_back(e);
    EdgeWrapper links = {e};
    _edge_map.insert({id, links});
    return e;
  }
};

template <Directedness D>
void AdjacencyListGraph<D>::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  _vertex_id_manager.free(v);
  _adj.erase(v);

  auto edge_map_it = _edge_map.begin();
  while (edge_map_it != _edge_map.end()) {
    auto [id, edge_wrapper] = *edge_map_it;
    auto edge = edge_wrapper[0];

    if (edge.u == v || edge.v == v) {
      edge_map_it = _edge_map.erase(edge_map_it);
    } else {
      edge_map_it++;
    }

    if (edge.v == v && edge.u != v) {
      _adj.at(edge.u).remove(edge);
    }

    if constexpr (D == Directedness::UNDIRECTED) {
      if (edge.u == v && edge.v != v) {
        _adj.at(edge.v).remove(edge);
      }
    }
  }
};

template <Directedness D>
void AdjacencyListGraph<D>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e)) {
    throw exceptions::NoSuchEdgeException();
  }

  _edge_id_manager.free(e);
  _adj.at(e.u).remove(e);

  if constexpr (D == Directedness::UNDIRECTED) {
    _adj.at(e.v).remove(e);
  }

  _edge_map.erase(e);
};

template <Directedness D> auto AdjacencyListGraph<D>::vertices() const {
  return _adj | std::views::transform([](std::pair<Id, EdgeList> pair) {
           return Vertex(pair.first);
         });
}

template <Directedness D> auto AdjacencyListGraph<D>::edges() const {
  return _edge_map | std::views::transform([](std::pair<Id, EdgeWrapper> pair) {
           return pair.second;
         }) |
         std::views::join;
}

template <Directedness D>
auto AdjacencyListGraph<D>::out_edges(const Vertex &v) const {
  return _adj.at(v) |
         std::views::transform([&](Id id) { return _edge_map.at(id); }) |
         std::views::join;
}

template <Directedness D>
auto AdjacencyListGraph<D>::in_edges(const Vertex &v) const {
  return _edge_map | std::views::transform([](std::pair<Id, EdgeWrapper> pair) {
           return pair.second;
         }) |
         std::views::join |
         std::views::filter([&](Edge edge) { return edge.v == v; });
}

template <Directedness D>
Vertex AdjacencyListGraph<D>::get_vertex(const Id &id) const {
  if (!_adj.contains(id)) {
    return INVALID_VERTEX;
  }
  return Vertex{id};
}

template <Directedness D>
Edge AdjacencyListGraph<D>::get_edge(const Id &id) const {
  if (!_edge_map.contains(id)) {
    return INVALID_EDGE;
  }
  return _edge_map.at(id)[0];
}

} // namespace graph