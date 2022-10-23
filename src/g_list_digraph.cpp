#include "g_list_digraph.hpp"
#include <iterator>

namespace graph {
// VertexContainer definition start
AdjacencyListDigraph::VertexContainer::VertexContainer(AdjacencyListDigraph *g)
    : _g{g} {};

AdjacencyListDigraph::VertexContainer::It
AdjacencyListDigraph::VertexContainer::begin() {
  return {_g->_adj.begin()};
}

AdjacencyListDigraph::VertexContainer::It
AdjacencyListDigraph::VertexContainer::end() {
  return {_g->_adj.end()};
}

AdjacencyListDigraph::VertexContainer::It::It(
    AdjacencyListDigraph::VIterator it)
    : _it{it} {}
Vertex AdjacencyListDigraph::VertexContainer::It::operator*() {
  auto id = (*_it).first;
  Vertex vertex{id};
  return vertex;
}

AdjacencyListDigraph::VertexContainer::It &
AdjacencyListDigraph::VertexContainer::It::operator++() {
  _it++;
  return *this;
}

AdjacencyListDigraph::VertexContainer::It
AdjacencyListDigraph::VertexContainer::It::operator++(int) {
  It tmp = *this;
  ++(*this);
  return tmp;
}

bool AdjacencyListDigraph::VertexContainer::It::operator==(const It &other) {
  return _it == other._it;
}

bool AdjacencyListDigraph::VertexContainer::It::operator!=(const It &other) {
  return _it != other._it;
}
// VertexContainer definition end

// EdgeContainer definition start
AdjacencyListDigraph::EdgeContainer::EdgeContainer(AdjacencyListDigraph *g)
    : _g{g} {};

AdjacencyListDigraph::EdgeContainer::It
AdjacencyListDigraph::EdgeContainer::begin() {
  return {_g->_edge_map.begin()};
}

AdjacencyListDigraph::EdgeContainer::It
AdjacencyListDigraph::EdgeContainer::end() {
  return {_g->_edge_map.end()};
}

AdjacencyListDigraph::EdgeContainer::It::It(AdjacencyListDigraph::EIterator it)
    : _it{it} {}

Edge AdjacencyListDigraph::EdgeContainer::It::operator*() {
  return (*_it).second;
}

AdjacencyListDigraph::EdgeContainer::It &
AdjacencyListDigraph::EdgeContainer::It::operator++() {
  _it++;
  return *this;
}

AdjacencyListDigraph::EdgeContainer::It
AdjacencyListDigraph::EdgeContainer::It::operator++(int) {
  It tmp = *this;
  ++(*this);
  return tmp;
}

bool AdjacencyListDigraph::EdgeContainer::It::operator==(const It &other) {
  return _it == other._it;
}

bool AdjacencyListDigraph::EdgeContainer::It::operator!=(const It &other) {
  return _it != other._it;
}
// EdgeContainer definition end

Vertex AdjacencyListDigraph::add_vertex() {
  while (_adj.contains(_next_vid)) {
    ++_next_vid;
  }

  Vertex v = {_next_vid};
  _adj.insert({_next_vid, {}});
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
  _edge_map.insert({e.id, e});
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

    if (edge.target = v.id) {
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
  return true;
}

AdjacencyListDigraph::VertexContainer AdjacencyListDigraph::vertices() {
  return {this};
}

AdjacencyListDigraph::EdgeContainer AdjacencyListDigraph::edges() {
  return {this};
};
} // namespace graph