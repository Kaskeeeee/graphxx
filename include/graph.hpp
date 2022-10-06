#pragma once
#include <bits/stdc++.h>

namespace graph {
template <typename F> struct GraphOperationResult {
  F first;
  bool second;
};

template <typename V, typename E> class IGraph {
public:
  virtual GraphOperationResult<V> add_vertex(const V &v = V()) = 0;
  virtual GraphOperationResult<E> add_edge(const V &v, const V &u,
                                           const E &e) = 0;
  virtual ~IGraph() = default;
};

template <typename V, typename E> class AdjacencyList : public IGraph<V, E> {
private:
  struct Vertex;
  struct Edge;

  using Representation = std::vector<std::list<Edge>>;
  using VertexMap = std::unordered_map<size_t, V>;

  Representation _adjacency_list;
  VertexMap _vertexes;

public:
  virtual GraphOperationResult<V> add_vertex(const V &v = V()) override {
    _adjacency_list.push_back(std::list<Edge>());
    _vertexes.insert({_adjacency_list.size() - 1, v});
  }

  virtual GraphOperationResult<E> add_edge(const V &v, const V &u,
                                           const E &e) override {
    size_t v_index = -1;
    size_t u_index = -1;
    for (auto &it : _vertexes) {
      if (it.second == v) {
        v_index = it.first;
      }

      if (it.second == u) {
        u_index = it.first;
      }
    }

    if (v_index == -1 || u_index == -1) {
      throw std::invalid_argument("Some vertexes were not in the graph");
    }

    for (auto &edge : _adjacency_list[v_index]) {
      if (edge.u == u) {
        return {-1, false};
      }
    }

    auto edge = Edge(v, u, e);
    _adjacency_list[v_index].push_back(edge);
    return {e, true};
  }

  virtual void print() {
    for (size_t i = 0; i < _adjacency_list.size(); i++) {
      std::cout << "Vertex " << _vertexes[i] << ": ";
      for (auto &elem : _adjacency_list[i]) {
        std::cout << " -> [" << elem.u << "]";
      }
      std::cout << "\n";
    }
  }

private:
  struct Edge {
    V v, u;
    E e;

    Edge(const V &v, const V &u, const E &e) : v{v}, u{u}, e{e} {};
  };
};

} // namespace graph
