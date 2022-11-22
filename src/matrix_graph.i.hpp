#include "exceptions.hpp"
#include "id_manager.hpp"
#include "matrix_graph.hpp"
#include <cassert>
#include <ranges>

namespace graph {
template <GraphType T>
AdjacencyMatrixGraph<T>::AdjacencyMatrixGraph()
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {};

template <GraphType T> Vertex AdjacencyMatrixGraph<T>::add_vertex() {
  auto id = _vertex_id_manager.allocate();

  Vertex v{id};
  _adj[v] = {};
  return v;
};

template <GraphType T>
Edge AdjacencyMatrixGraph<T>::add_edge(const Vertex &u, const Vertex &v) {
  if (!_adj.contains(u) || !_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  auto id = _edge_id_manager.allocate();

  Edge e{id, u, v};
  _adj[u][v] = id;

  if constexpr (T == GraphType::Undirected) {
    _adj[v][u] = id;
  }

  _edge_map[id] = e;
  return e;
}

template <GraphType T>
void AdjacencyMatrixGraph<T>::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  _adj.erase(v);

  auto it = _edge_map.begin();
  while (it != _edge_map.end()) {
    if (it->second.u == v || it->second.v == v) {
      it = _edge_map.erase(it);
    } else {
      ++it;
    }
  }

  for (auto [_, id_map] : _adj) {
    id_map.erase(v);
  }
}

template <GraphType T>
void AdjacencyMatrixGraph<T>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e)) {
    throw exceptions::NoSuchEdgeException();
  }

  _edge_map.erase(e);
  _adj.at(e.u).erase(e.v);

  if constexpr (T == GraphType::Undirected) {
    _adj.at(e.v).erase(e.u);
  }
}

template <GraphType T> auto AdjacencyMatrixGraph<T>::vertices() {
  return _adj | std::views::transform(
                    [](std::pair<Id, std::unordered_map<Id, Id>> pair) {
                      return Vertex{pair.first};
                    });
}

template <GraphType T> auto AdjacencyMatrixGraph<T>::edges() {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
};

template <GraphType T>
auto AdjacencyMatrixGraph<T>::out_edges(const Vertex &v) {
  return _adj.at(v) | std::views::transform([&](std::pair<Id, Id> pair) {
           return _edge_map[pair.second];
         });
}

template <GraphType T> auto AdjacencyMatrixGraph<T>::in_edges(const Vertex &v) {
  return _adj |
         std::views::filter(
             [&](std::pair<Id, std::unordered_map<Id, Id>> pair) {
               return pair.second.contains(v);
             }) |
         std::views::transform(
             [&](std::pair<Id, std::unordered_map<Id, Id>> pair) {
               return _edge_map[pair.second[v]];
             });
}
} // namespace graph