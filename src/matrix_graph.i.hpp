#include "exceptions.hpp"
#include "id_manager.hpp"
#include "matrix_graph.hpp"
#include <cassert>
#include <ranges>

namespace graph {
template <Directedness D>
AdjacencyMatrixGraph<D>::AdjacencyMatrixGraph()
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {};

template <Directedness D>
AdjacencyMatrixGraph<D>::AdjacencyMatrixGraph(const AdjacencyMatrixGraph &graph)
    : _vertex_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)},
      _edge_id_manager{utils::IdManager(MIN_VALID_ID, MAX_VALID_ID)} {

  for (Vertex v : graph.vertices) {
    _adj[v] = {};
  }

  for (Edge e : graph.edges()) {
    add_edge(e.u, e.v);
  }
};

template <Directedness D> Vertex AdjacencyMatrixGraph<D>::add_vertex() {
  auto id = _vertex_id_manager.allocate();

  Vertex v{id};
  _adj[v] = {};
  return v;
};

template <Directedness D>
Edge AdjacencyMatrixGraph<D>::add_edge(const Vertex &u, const Vertex &v) {
  if (!_adj.contains(u) || !_adj.contains(v)) {
    throw exceptions::NoSuchVertexException();
  }

  auto id = _edge_id_manager.allocate();

  Edge e{id, u, v};
  _adj[u][v] = id;

  if constexpr (D == Directedness::UNDIRECTED) {
    _adj[v][u] = id;
  }

  _edge_map[id] = e;
  return e;
}

template <Directedness D>
void AdjacencyMatrixGraph<D>::remove_vertex(const Vertex &v) {
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

template <Directedness D>
void AdjacencyMatrixGraph<D>::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e)) {
    throw exceptions::NoSuchEdgeException();
  }

  _edge_map.erase(e);
  _adj.at(e.u).erase(e.v);

  if constexpr (D == Directedness::UNDIRECTED) {
    _adj.at(e.v).erase(e.u);
  }
}

template <Directedness D> auto AdjacencyMatrixGraph<D>::vertices() const {
  return _adj | std::views::transform(
                    [](const std::pair<Id, std::unordered_map<Id, Id>> &pair) {
                      return Vertex{pair.first};
                    });
}

template <Directedness D> auto AdjacencyMatrixGraph<D>::edges() const {
  return _edge_map | std::views::transform(
                         [](std::pair<Id, Edge> pair) { return pair.second; });
};

template <Directedness D>
auto AdjacencyMatrixGraph<D>::out_edges(const Vertex &v) const {
  return _adj.at(v) | std::views::transform([&](std::pair<Id, Id> pair) {
           return _edge_map.at(pair.second);
         });
}

template <Directedness D>
auto AdjacencyMatrixGraph<D>::in_edges(const Vertex &v) const {
  return _adj |
         std::views::filter(
             [&](const std::pair<Id, std::unordered_map<Id, Id>> &pair) {
               return pair.second.contains(v);
             }) |
         std::views::transform(
             [&](const std::pair<Id, std::unordered_map<Id, Id>> &pair) {
               return _edge_map.at(pair.second.at(v));
             });
}

template <Directedness D>
Vertex AdjacencyMatrixGraph<D>::get_vertex(const Id &id) const {
  if (!_adj.contains(id)) {
    return INVALID_VERTEX;
  }
  return Vertex{id};
}

template <Directedness D>
Edge AdjacencyMatrixGraph<D>::get_edge(const Id &id) const {
  if (!_edge_map.contains(id)) {
    return INVALID_EDGE;
  }
  return _edge_map.at(id);
}
} // namespace graph