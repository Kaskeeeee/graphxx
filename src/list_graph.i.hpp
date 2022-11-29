#pragma once
#include "base.hpp"
#include "exceptions.hpp"
#include "id_manager.hpp"
#include "list_graph.hpp"
#include <cassert>
#include <type_traits>

#include <ranges>

namespace graph {
template <concepts::Orientable GraphType>
AdjacencyListGraph<GraphType>::AdjacencyListGraph()
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {};

template <concepts::Orientable GraphType>
Vertex AdjacencyListGraph<GraphType>::add_vertex() {
  auto id = _vertex_id_manager.allocate();

  Vertex v{id};
  _adj[v] = {};
  return v;
};

template <concepts::Orientable GraphType>
Edge AdjacencyListGraph<GraphType>::add_edge(const Vertex &u, const Vertex &v) {
  if (!_adj.contains(u) || !_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  auto id = _edge_id_manager.allocate();
  Edge e{id, u, v};

  if constexpr (std::is_same_v<GraphType, UndirectedGraph>) {
    _adj[v].push_back(e);
  }

  _adj[u].push_back(e);
  _edge_map[e] = e;
  return e;
};

template <concepts::Orientable GraphType>
void AdjacencyListGraph<GraphType>::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  _vertex_id_manager.free(v);
  _adj.erase(v);

  auto edge_map_it = _edge_map.begin();
  while (edge_map_it != _edge_map.end()) {
    auto [id, edge] = *edge_map_it;
    if (edge.u == v || edge.v == v) {
      edge_map_it = _edge_map.erase(edge_map_it);
    } else {
      edge_map_it++;
    }

    if (edge.v == v && edge.u != v) {
      _adj.at(edge.u).remove(edge);
    }

    if constexpr (std::is_same_v<GraphType, UndirectedGraph>) {
      if (edge.u == v && edge.v != v) {
        _adj.at(edge.v).remove(edge);
      }
    }
  }
};

template <concepts::Orientable GraphType>
void AdjacencyListGraph<GraphType>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e)) {
    throw exceptions::NoSuchEdgeException();
  }

  _edge_id_manager.free(e);
  _adj.at(e.u).remove(e);

  if constexpr (std::is_same_v<GraphType, UndirectedGraph>) {
    _adj.at(e.v).remove(e);
  }

  _edge_map.erase(e);
};

template <concepts::Orientable GraphType>
auto AdjacencyListGraph<GraphType>::vertices() const {
  return _adj | std::views::transform([](std::pair<Id, EdgeList> pair) {
           return Vertex(pair.first);
         });
}

template <concepts::Orientable GraphType>
auto AdjacencyListGraph<GraphType>::edges() const {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
}

template <concepts::Orientable GraphType>
auto AdjacencyListGraph<GraphType>::out_edges(const Vertex &v) const {
  return _adj.at(v) |
         std::views::transform([&](Id id) { return _edge_map.at(id); });
}

template <concepts::Orientable GraphType>
auto AdjacencyListGraph<GraphType>::in_edges(const Vertex &v) const {
  return _edge_map | std::views::filter([&](std::pair<Id, Edge> pair) {
           return pair.second.v == v;
         }) |
         std::views::transform(
             [](std::pair<Id, Edge> pair) { return pair.second; });
}

} // namespace graph