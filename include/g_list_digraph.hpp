#pragma once
#include "g_base.hpp"
#include "g_container.hpp"
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

public:
  Vertex add_vertex();
  Edge add_edge(const Vertex &, const Vertex &);
  bool remove_vertex(const Vertex &);
  bool remove_edge(const Edge &);

  container::Container<VertexMapIt> vertices();
  container::Container<EdgeMapIt> edges();
  container::Container<OutEdgeMapIt> out_edges(const Vertex &);
  container::Container<InEdgeMapIt> in_edges(const Vertex &);

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
class AdjacencyListDigraph::EdgeMapIt
    : public it::MapIterator<EdgeMapIt, Edge, EdgeMap::iterator> {
public:
  void next();
  Edge value();
  EdgeMapIt(EdgeMap::iterator it);
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

} // namespace graph