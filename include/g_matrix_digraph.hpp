#pragma once
#include "g_base.hpp"
#include "g_container.hpp"
#include "g_iterator.hpp"

#include <unordered_map>

namespace graph {

class AdjacencyMatrixDigraph {
protected:
  using AdjacencyMap = std::unordered_map<Id, Id>;
  using AdjacencyMatrix = std::unordered_map<Id, AdjacencyMap>;
  using EdgeMap = std::unordered_map<Id, Edge>;

  class VertexMapIt;
  class EdgeMapIt;
  class OutEdgeMapIt;
  class InEdgeMapIt;

  friend class VertexMapIt;
  friend class EdgeMapIt;
  friend class OutEdgeMapIt;
  friend class InEdgeMapIt;

  using Vertices = container::C<VertexMapIt, AdjacencyMatrix>;
  using Edges = container::C<EdgeMapIt, EdgeMap>;
  using OutEdges = container::C<OutEdgeMapIt, AdjacencyMap, EdgeMap>;
  using InEdges = container::C<InEdgeMapIt, AdjacencyMatrix, AdjacencyMatrix, EdgeMap, Vertex>;

public:
  Vertex add_vertex();
  Edge add_edge(const Vertex &, const Vertex &);
  bool remove_vertex(const Vertex &);
  bool remove_edge(const Edge &);

  Vertices vertices();
  Edges edges();
  OutEdges out_edges(const Vertex &);
  InEdges in_edges(const Vertex &);

private:
  AdjacencyMatrix _adj;
  EdgeMap _edge_map;

  Id _next_vid = 0;
  Id _next_eid = 0;
};

class AdjacencyMatrixDigraph::VertexMapIt
    : public it::MapIterator<VertexMapIt, Vertex, AdjacencyMatrix::iterator> {
public:
  void next();
  Vertex value();
  VertexMapIt(AdjacencyMatrix::iterator it);
};

class AdjacencyMatrixDigraph::EdgeMapIt
    : public it::MapIterator<EdgeMapIt, Edge, EdgeMap::iterator> {
public:
  void next();
  Edge value();
  EdgeMapIt(EdgeMap::iterator it);
};

class AdjacencyMatrixDigraph::OutEdgeMapIt
    : public it::MapIterator<OutEdgeMapIt, Edge, AdjacencyMap::iterator> {
    public:
        void next();
        Edge value();
        OutEdgeMapIt(AdjacencyMap::iterator it, EdgeMap &edge_map);

    private:
        EdgeMap &_edge_map;
};

class AdjacencyMatrixDigraph::InEdgeMapIt
    : public it::MapIterator<InEdgeMapIt, Edge, AdjacencyMatrix::iterator> {
    public:
        void next();
        Edge value();
        InEdgeMapIt(AdjacencyMatrix::iterator it, AdjacencyMatrix &_adj, EdgeMap &edge_map, const Vertex &target);

    private:
        AdjacencyMatrix &_adj;
        EdgeMap &_edge_map;
        Vertex _target;
};

} // namespace graph