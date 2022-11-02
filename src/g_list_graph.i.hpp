#pragma once
#include "g_base.hpp"
#include "g_list_graph.hpp"
#include <ranges>

namespace graph {
template <GraphType T> Vertex AdjacencyListGraph<T>::add_vertex() {
  while (_adj.contains(_next_vid)) {
    ++_next_vid;
  }

  Vertex v{_next_vid};
  _adj[_next_vid] = {};
  ++_next_vid;
  return v;
};

template <GraphType T>
Edge AdjacencyListGraph<T>::add_edge(const Vertex &u, const Vertex &v) {
  while (_edge_map.contains(_next_eid)) {
    ++_next_eid;
  }

  Edge e{_next_eid, u.id, v.id};
  ++_next_eid;

  if constexpr (T == GraphType::Directed) {
    _adj[v.id];
  } else {
    _adj[v.id].push_back(e.id);
  }

  _adj[u.id].push_back(e.id);
  _edge_map[e.id] = e;
  return e;
};

template <GraphType T>
bool AdjacencyListGraph<T>::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v.id)) {
    return false;
  }

  _adj.erase(v.id);

  auto edge_map_it = _edge_map.begin();
  while (edge_map_it != _edge_map.end()) {
    auto [id, edge] = *edge_map_it;
    if (edge.source == v.id || edge.target == v.id) {
      edge_map_it = _edge_map.erase(edge_map_it);
    } else {
      edge_map_it++;
    }

    if (edge.target == v.id) {
      _adj.at(edge.source).remove(edge.id);
    }

    if constexpr (T == GraphType::Undirected) {
      if (edge.source == v.id) {
        _adj.at(edge.target).remove(edge.id);
      }
    }
  }

  return true;
};

template <GraphType T> bool AdjacencyListGraph<T>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e.id)) {
    return false;
  }

  auto edge = _edge_map.at(e.id);
  _adj.at(edge.source).remove(e.id);

  if constexpr (T == GraphType::Undirected) {
    _adj.at(edge.target).remove(e.id);
  }

  _edge_map.erase(e.id);
  return true;
};

template <GraphType T> decltype(auto) AdjacencyListGraph<T>::vertices() {
  return _adj | std::views::transform([](std::pair<Id, EdgeList> pair) {
           return Vertex(pair.first);
         });
}

template <GraphType T> decltype(auto) AdjacencyListGraph<T>::edges() {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
}

template <GraphType T>
decltype(auto) AdjacencyListGraph<T>::out_edges(const Vertex &v) {
  return _adj.at(v.id) |
         std::views::transform([&](Id id) { return _edge_map[id]; });
}

template <GraphType T>
decltype(auto) AdjacencyListGraph<T>::in_edges(const Vertex &v) {
  return _edge_map | std::views::filter([&](std::pair<Id, Edge> pair) {
           return pair.second.target == v.id;
         }) |
         std::views::transform(
             [](std::pair<Id, Edge> pair) { return pair.second; });
}

} // namespace graph