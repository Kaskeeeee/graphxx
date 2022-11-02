#include "g_matrix_graph.hpp"
#include <ranges>

namespace graph {
template <GraphType T> Vertex AdjacencyMatrixGraph<T>::add_vertex() {
  while (_adj.contains(_next_vid)) {
    ++_next_vid;
  }

  Vertex v{_next_vid};
  _adj[_next_vid] = {};
  ++_next_vid;
  return v;
};

template <GraphType T>
Edge AdjacencyMatrixGraph<T>::add_edge(const Vertex &u, const Vertex &v) {
  while (_edge_map.contains(_next_eid)) {
    ++_next_eid;
  }

  Edge e{_next_eid, u.id, v.id};
  _adj[u.id][v.id] = _next_eid;

  if constexpr (T == GraphType::Undirected) {
    _adj[v.id][u.id] = _next_eid;
  }

  ++_next_eid;
  _edge_map[e.id] = e;
  return e;
}

template <GraphType T>
bool AdjacencyMatrixGraph<T>::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v.id)) {
    return false;
  }

  _adj.erase(v.id);

  auto it = _edge_map.begin();
  while (it != _edge_map.end()) {
    if (it->second.source == v.id || it->second.target == v.id) {
      it = _edge_map.erase(it);
    } else {
      ++it;
    }
  }

  for (auto [_, id_map] : _adj) {
    id_map.erase(v.id);
  }

  return true;
}

template <GraphType T>
bool AdjacencyMatrixGraph<T>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e.id)) {
    return false;
  }

  _edge_map.erase(e.id);
  _adj.at(e.source).erase(e.target);

  if constexpr (T == GraphType::Undirected) {
    _adj.at(e.target).erase(e.source);
  }

  return true;
}

template <GraphType T> decltype(auto) AdjacencyMatrixGraph<T>::vertices() {
  return _adj | std::views::transform(
                    [](std::pair<Id, std::unordered_map<Id, Id>> pair) {
                      return Vertex{pair.first};
                    });
}

template <GraphType T> decltype(auto) AdjacencyMatrixGraph<T>::edges() {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
};

template <GraphType T>
decltype(auto) AdjacencyMatrixGraph<T>::out_edges(const Vertex &v) {
  return _adj.at(v.id) | std::views::transform([&](std::pair<Id, Id> pair) {
           return _edge_map[pair.second];
         });
}

template <GraphType T>
decltype(auto) AdjacencyMatrixGraph<T>::in_edges(const Vertex &v) {
  return _adj |
         std::views::filter(
             [&](std::pair<Id, std::unordered_map<Id, Id>> pair) {
               return pair.second.contains(v.id);
             }) |
         std::views::transform(
             [&](std::pair<Id, std::unordered_map<Id, Id>> pair) {
               return _edge_map[pair.second[v.id]];
             });
}
} // namespace graph