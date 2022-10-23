#pragma once
#include "g_base.hpp"
#include "g_walker.hpp"

#include <list>
#include <unordered_map>
#include <unordered_set>

namespace graph {
class AdjacencyListDigraph {
protected:
  using AdjacencyList = std::unordered_map<Id, std::list<Id>>;
  using EdgeMap = std::unordered_map<Id, Edge>;
  using VIterator = AdjacencyList::iterator;
  using EIterator = EdgeMap::iterator;

  struct VertexContainer {
    struct It {
      using value_type = Vertex;
      using reference = Vertex &;
      using pointer = Vertex *;

      It(VIterator);

      Vertex operator*();
      It &operator++();
      It operator++(int);
      bool operator==(const It &);
      bool operator!=(const It &);

    private:
      VIterator _it;
    };

    VertexContainer(AdjacencyListDigraph *g);
    It begin();
    It end();

  private:
    AdjacencyListDigraph *_g;
  };

  struct EdgeContainer {
    struct It {
      using value_type = Edge;
      using reference = Edge &;
      using pointer = Edge *;

      It(EIterator);

      Edge operator*();
      It &operator++();
      It operator++(int);
      bool operator==(const It &);
      bool operator!=(const It &);

    private:
      EIterator _it;
    };

    EdgeContainer(AdjacencyListDigraph *g);
    It begin();
    It end();

  private:
    AdjacencyListDigraph *_g;
  };

public:
  Vertex add_vertex();
  Edge add_edge(const Vertex &, const Vertex &);
  bool remove_vertex(const Vertex &);
  bool remove_edge(const Edge &);
  VertexContainer vertices();
  EdgeContainer edges();

private:
  AdjacencyList _adj;
  EdgeMap _edge_map;

  Id _next_vid = 0;
  Id _next_eid = 0;
};

template <>
class walker::Walker<AdjacencyListDigraph, Vertex,
                     AdjacencyListDigraph::AdjacencyList::iterator>;

} // namespace graph