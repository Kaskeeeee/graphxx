#pragma once
#include <bits/stdc++.h>

namespace graph {
template <typename F> struct GraphOperationResult {
  F first;
  bool second;
};

struct EmptyData {};

using VertexId = uint32_t;
using EdgeId = uint32_t;

template <typename D> struct Vertex {
  VertexId id;
  D data;

  bool operator==(const Vertex &other) { return other.id == id; }
};

template <typename D> struct Edge {
  EdgeId id;
  VertexId source;
  VertexId target;
  D data;

  bool operator==(const Edge &other) {
    return other.source == source && other.target == target && other.id == id;
  }
};

template <typename V, typename E> class IGraph {
private:
  using VT = Vertex<V>;
  using ET = Edge<E>;

public:
  virtual GraphOperationResult<Vertex<V>> add_vertex(const V &v) = 0;
  virtual GraphOperationResult<Edge<E>> add_edge(const VT &v, const VT &u,
                                                 const E &e) = 0;
  virtual void remove_vertex(const VT &v) = 0;
  virtual void remove_edge(const ET &e) = 0;
  virtual VT &get_vertex_by_id(uint32_t id) = 0;

  virtual ~IGraph() = default;
};

template <typename V = EmptyData, typename E = EmptyData>
class AdjacencyList : public IGraph<V, E> {
private:
  using VertexType = Vertex<V>;
  using EdgeType = Edge<E>;
  using GraphRepresentation = std::unordered_map<VertexId, std::list<EdgeId>>;
  using VertexMap = std::unordered_map<VertexId, VertexType>;
  using EdgeMap = std::unordered_map<EdgeId, EdgeType>;

  GraphRepresentation _adj_list;
  VertexMap _vertexes;
  EdgeMap _edges;
  VertexId next_vid = 0;
  EdgeId next_eid = 0;

public:
  GraphRepresentation get_adj_list() { return _adj_list; }

  virtual GraphOperationResult<VertexType> add_vertex(const V &v) override {
    while (_vertexes.contains(next_vid)) {
      ++next_vid;
    }

    VertexType vertex = {next_vid, v};

    _vertexes.insert({next_vid, vertex});
    _adj_list.insert({next_vid, {}});

    ++next_vid;
    return {vertex, true};
  }

  virtual GraphOperationResult<EdgeType> add_edge(const VertexType &v,
                                                  const VertexType &u,
                                                  const E &e = E()) override {
    while (_edges.contains(next_eid)) {
      ++next_eid;
    }

    EdgeType edge = {next_eid, v.id, u.id, e};

    _edges.insert({next_eid, edge});
    _adj_list[v.id].push_back(next_eid);

    ++next_eid;
    return {edge, true};
  }

  virtual void remove_vertex(const VertexType &v) override {
    _adj_list.erase(v.id);

    auto it = _edges.begin();
    while (it != _edges.end()) {
      auto edge = it->second;
      if (edge.target == v.id && edge.source != v.id) {
        _adj_list[edge.source].remove(edge.id);
      }

      if (edge.source == v.id || edge.target == v.id) {
        it = _edges.erase(it);
      } else {
        ++it;
      }
    }

    _vertexes.erase(v.id);
  }

  virtual void remove_edge(const EdgeType &e) override {
    _adj_list[e.source].remove(e.id);
    _edges.erase(e.id);
  }

  virtual VertexType &get_vertex_by_id(VertexId id) { return _vertexes[id]; }

  virtual void print() {
    for (auto const &key_list : _adj_list) {
      std::cout << "Vertex " << _vertexes[key_list.first].id << ":";
      for (auto const &edge : key_list.second) {
        std::cout << " -> [" << _edges[edge].target << "]";
      }
      std::cout << "\n";
    }
  }
};

} // namespace graph
