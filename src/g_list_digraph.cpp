#include "g_list_digraph.hpp"
#include <iterator>

namespace graph {
Vertex AdjacencyListDigraph::add_vertex() {
  while (_adj.contains(_next_vid)) {
    ++_next_vid;
  }

  Vertex v = {_next_vid};
  _adj[_next_vid] = {};
  ++_next_vid;
  return v;
};

Edge AdjacencyListDigraph::add_edge(const Vertex &u, const Vertex &v) {
  while (_edge_map.contains(_next_eid)) {
    ++_next_eid;
  }

  Edge e = {_next_eid, u.id, v.id};
  ++_next_eid;
  _adj.at(u.id).push_back(e.id);
  _edge_map[e.id] = e;
  return e;
}

bool AdjacencyListDigraph::remove_vertex(const Vertex &v) {
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
  }

  return true;
}

bool AdjacencyListDigraph::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e.id)) {
    return false;
  }

  auto edge = _edge_map.at(e.id);
  _adj.at(edge.source).remove(e.id);
  _edge_map.erase(e.id);
  return true;
}

// VertexMapIt
void AdjacencyListDigraph::VertexMapIt::next() { ++_it; };
Vertex AdjacencyListDigraph::VertexMapIt::value() { return Vertex{_it->first}; }
AdjacencyListDigraph::VertexMapIt::VertexMapIt(AdjacencyList::iterator it)
    : _Super{it} {}

// EdgeMapIt
void AdjacencyListDigraph::EdgeMapIt::next() { ++_it; }
Edge AdjacencyListDigraph::EdgeMapIt::value() { return _it->second; }
AdjacencyListDigraph::EdgeMapIt::EdgeMapIt(EdgeMap::iterator it)
    : _Super{it} {};

// OutEdgeMapIt
void AdjacencyListDigraph::OutEdgeMapIt::next() { ++_it; }
Edge AdjacencyListDigraph::OutEdgeMapIt::value() { return _edge_map.at(*_it); }
AdjacencyListDigraph::OutEdgeMapIt::OutEdgeMapIt(EdgeList::iterator it,
                                                 EdgeMap &edge_map)
    : _Super{it}, _edge_map{edge_map} {};

// InEdgeMapIt
void AdjacencyListDigraph::InEdgeMapIt::next() {
  while (_it != _edge_map.end()) {
    _it++;
    if (_it->second.target == _target.id) {
      return;
    }
  }
}
Edge AdjacencyListDigraph::InEdgeMapIt::value() { return _it->second; }
AdjacencyListDigraph::InEdgeMapIt::InEdgeMapIt(EdgeMap::iterator it,
                                               EdgeMap &edge_map,
                                               const Vertex &target)
    : _Super{it}, _edge_map{edge_map}, _target{target} {};

AdjacencyListDigraph::Vertices AdjacencyListDigraph::vertices() {
  return {_adj};
}

AdjacencyListDigraph::Edges AdjacencyListDigraph::edges() {
  return {_edge_map};
};

AdjacencyListDigraph::OutEdges
AdjacencyListDigraph::out_edges(const Vertex &v) {
  return {_adj.at(v.id), _edge_map};
}

AdjacencyListDigraph::InEdges AdjacencyListDigraph::in_edges(const Vertex &v) {
  return {_edge_map, _edge_map, v};
}
} // namespace graph