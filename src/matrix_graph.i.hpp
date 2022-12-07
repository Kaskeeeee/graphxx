#include "exceptions.hpp"
#include "id_manager.hpp"
#include "matrix_graph.hpp"
#include <cassert>
#include <ranges>

namespace graph {
template <concepts::Directable GraphType>
AdjacencyMatrixGraph<GraphType>::AdjacencyMatrixGraph()
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {};

template <concepts::Directable GraphType>
Vertex AdjacencyMatrixGraph<GraphType>::add_vertex() {
  auto id = _vertex_id_manager.allocate();

  Vertex v{id};
  _adj[v] = {};
  return v;
};

template <concepts::Directable GraphType>
Edge AdjacencyMatrixGraph<GraphType>::add_edge(const Vertex &u,
                                               const Vertex &v) {
  if (!_adj.contains(u) || !_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  auto id = _edge_id_manager.allocate();

  Edge e{id, u, v};
  _adj[u][v] = id;

  if constexpr (std::is_same_v<GraphType, UndirectedGraph>) {
    _adj[v][u] = id;
  }

  _edge_map[id] = e;
  return e;
}

template <concepts::Directable GraphType>
void AdjacencyMatrixGraph<GraphType>::remove_vertex(const Vertex &v) {
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

template <concepts::Directable GraphType>
void AdjacencyMatrixGraph<GraphType>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e)) {
    throw exceptions::NoSuchEdgeException();
  }

  _edge_map.erase(e);
  _adj.at(e.u).erase(e.v);

  if constexpr (std::is_same_v<GraphType, UndirectedGraph>) {
    _adj.at(e.v).erase(e.u);
  }
}

template <concepts::Directable GraphType>
auto AdjacencyMatrixGraph<GraphType>::vertices() const {
  return _adj | std::views::transform(
                    [](std::pair<Id, std::unordered_map<Id, Id>> pair) {
                      return Vertex{pair.first};
                    });
}

template <concepts::Directable GraphType>
auto AdjacencyMatrixGraph<GraphType>::edges() const {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
};

template <concepts::Directable GraphType>
auto AdjacencyMatrixGraph<GraphType>::out_edges(const Vertex &v) const {
  return _adj.at(v) | std::views::transform([&](std::pair<Id, Id> pair) {
           return _edge_map[pair.second];
         });
}

template <concepts::Directable GraphType>
auto AdjacencyMatrixGraph<GraphType>::in_edges(const Vertex &v) const {
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

template <concepts::Directable GraphType>
Vertex AdjacencyMatrixGraph<GraphType>::get_vertex(const Id &id) const {
  if (!_adj.contains(id)) {
    throw exceptions::NoSuchVertexException();
  }
  return Vertex{id};
}

template <concepts::Directable GraphType>
Edge AdjacencyMatrixGraph<GraphType>::get_edge(const Id &id) const {
  if (!_edge_map.contains(id)) {
    throw exceptions::NoSuchEdgeException();
  }
  return _edge_map.at(id);
}
} // namespace graph