#pragma once
#include "g_base.hpp"
#include "g_iterator.hpp"

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace graph {

class AdjacencyListDigraph {
protected:
  using EdgeList = std::list<Id>;
  using AdjacencyList = std::unordered_map<Id, EdgeList>;
  using EdgeMap = std::unordered_map<Id, Edge>;

  class VertexMapIt;
  class EdgeMapIt;
  class OutEdgeMapIt;
  class InEdgeMapIt;

  friend class VertexMapIt;
  friend class EdgeMapIt;
  friend class OutEdgeMapIt;
  friend class InEdgeMapIt;

  struct Container1;
  struct Container2;
  struct Container3;
  struct Container4;

public:
  Vertex add_vertex();
  Edge add_edge(const Vertex &, const Vertex &);
  bool remove_vertex(const Vertex &);
  bool remove_edge(const Edge &);

  Container1 vertices();
  Container2 edges();
  Container3 out_edges(const Vertex &);
  Container4 in_edges(const Vertex &);

private:
  AdjacencyList _adj;
  EdgeMap _edge_map;

  Id _next_vid = 0;
  Id _next_eid = 0;
};

class AdjacencyListDigraph::VertexMapIt
    : public it::MapIterator<VertexMapIt, Vertex, AdjacencyList::iterator> {
public:
  void next();
  Vertex value();
  VertexMapIt(AdjacencyList::iterator it);
};

struct AdjacencyListDigraph::Container1 {
  VertexMapIt begin() { return VertexMapIt(_adj.begin()); }
  VertexMapIt end() { return VertexMapIt(_adj.end()); }

  Container1(AdjacencyList &adj) : _adj{adj} {};

  AdjacencyList &_adj;
};

class AdjacencyListDigraph::EdgeMapIt
    : public it::MapIterator<EdgeMapIt, Edge, EdgeMap::iterator> {
public:
  void next();
  Edge value();
  EdgeMapIt(EdgeMap::iterator it);
};

struct AdjacencyListDigraph::Container2 {
  EdgeMapIt begin() { return EdgeMapIt(_map.begin()); }
  EdgeMapIt end() { return EdgeMapIt(_map.end()); }

  Container2(EdgeMap &map) : _map{map} {};

  EdgeMap &_map;
};

class AdjacencyListDigraph::OutEdgeMapIt
    : public it::MapIterator<OutEdgeMapIt, Edge, EdgeList::iterator> {
public:
  void next();
  Edge value();
  OutEdgeMapIt(EdgeList::iterator it, EdgeMap &edge_map);

private:
  EdgeMap &_edge_map;
};

struct AdjacencyListDigraph::Container3 {
  OutEdgeMapIt begin() { return OutEdgeMapIt(_list.begin(), _map); }
  OutEdgeMapIt end() { return OutEdgeMapIt(_list.end(), _map); }

  Container3(EdgeList &list, EdgeMap &map) : _list{list}, _map{map} {};

  EdgeList &_list;
  EdgeMap &_map;
};

class AdjacencyListDigraph::InEdgeMapIt
    : public it::MapIterator<InEdgeMapIt, Edge, EdgeMap::iterator> {
public:
  void next();
  Edge value();
  InEdgeMapIt(EdgeMap::iterator it, EdgeMap &edge_map, const Vertex &target);

private:
  EdgeMap &_edge_map;
  Vertex _target;
};

struct AdjacencyListDigraph::Container4 {
  InEdgeMapIt begin() { return InEdgeMapIt(_map.begin(), _map, _v); }
  InEdgeMapIt end() { return InEdgeMapIt(_map.end(), _map, _v); }

  Container4(EdgeMap &map, Vertex v) : _map{map}, _v{v} {};

  EdgeMap &_map;
  Vertex _v;
};

} // namespace graph