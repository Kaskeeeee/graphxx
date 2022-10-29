#include "g_matrix_digraph.hpp"
#include <iterator>

namespace graph {
Vertex AdjacencyMatrixDigraph::add_vertex() {
  while (_adj.contains(_next_vid)) {
    ++_next_vid;
  }

  Vertex v = {_next_vid};
  _adj[_next_vid] = {};
  ++_next_vid;
  return v;
};

Edge AdjacencyMatrixDigraph::add_edge(const Vertex &u, const Vertex &v) {
  while (_edge_map.contains(_next_eid)) {
    ++_next_eid;
  }

  Edge e = {_next_eid, u.id, v.id};
  _adj[u.id][v.id] = _next_eid;
  ++_next_eid;
  _edge_map[e.id] = e;
  return e;
}

bool AdjacencyMatrixDigraph::remove_vertex(const Vertex &v) {
  if (!_adj.contains(v.id)) {
    return false;
  }

  _adj.erase(v.id);

  auto it = _edge_map.begin();
  while(it != _edge_map.end()){
    if(it->second.source == v.id || it->second.target == v.id){
        it = _edge_map.erase(it);
    }
    else{
        ++it;
    }
  }

  for(auto [_, mapId] : _adj){
    mapId.erase(v.id);
  }

  return true;
}

bool AdjacencyMatrixDigraph::remove_edge(const Edge &e) {
  if (!_edge_map.contains(e.id)) {
    return false;
  }

  _edge_map.erase(e.id);
  _adj.at(e.source).erase(e.target);
  return true;
}

// VertexMapIt
void AdjacencyMatrixDigraph::VertexMapIt::next() { ++_it; };
Vertex AdjacencyMatrixDigraph::VertexMapIt::value() { return Vertex{_it->first}; }
AdjacencyMatrixDigraph::VertexMapIt::VertexMapIt(AdjacencyMatrix::iterator it)
    : _Super{it} {}

// EdgeMapIt
void AdjacencyMatrixDigraph::EdgeMapIt::next() { ++_it; }
Edge AdjacencyMatrixDigraph::EdgeMapIt::value() { return _it->second; }
AdjacencyMatrixDigraph::EdgeMapIt::EdgeMapIt(EdgeMap::iterator it)
    : _Super{it} {};

// OutEdgeMapIt
void AdjacencyMatrixDigraph::OutEdgeMapIt::next() { ++_it; }
Edge AdjacencyMatrixDigraph::OutEdgeMapIt::value() { return _edge_map.at(_it->second); }
AdjacencyMatrixDigraph::OutEdgeMapIt::OutEdgeMapIt(AdjacencyMap::iterator it,
                                                EdgeMap &edge_map)
    : _Super{it}, _edge_map{edge_map} {};

// InEdgeMapIt
void AdjacencyMatrixDigraph::InEdgeMapIt::next() {
  while (_it != _adj.end()) {
    _it++;
    if (_it->second.contains(_target.id)) {
      return;
    }
  }
}
Edge AdjacencyMatrixDigraph::InEdgeMapIt::value() { return _edge_map.at(_it->second.at(_target.id)); }
AdjacencyMatrixDigraph::InEdgeMapIt::InEdgeMapIt(AdjacencyMatrix::iterator it,
                                               AdjacencyMatrix &_adj,
                                               EdgeMap &edge_map,
                                               const Vertex &target)
    : _Super{it}, _adj{_adj}, _edge_map{edge_map}, _target{target} {};

AdjacencyMatrixDigraph::Vertices AdjacencyMatrixDigraph::vertices() {
  return {_adj};
}

AdjacencyMatrixDigraph::Edges AdjacencyMatrixDigraph::edges() {
  return {_edge_map};
};

AdjacencyMatrixDigraph::OutEdges AdjacencyMatrixDigraph::out_edges(const Vertex &v) {
  return {_adj.at(v.id), _edge_map};
}

AdjacencyMatrixDigraph::InEdges AdjacencyMatrixDigraph::in_edges(const Vertex &v) {
  return {_adj, _adj, _edge_map, v};
}
} // namespace graph