#include "algorithms/tarjan.hpp"
#include "base.hpp"
#include "graph_concepts.hpp"

#include <algorithm>

namespace graph::algorithms::tarjan {

SCCVector _scc_vector;
TarjanTree _tarjan_tree;
StackVector _stack;
int index;

template <concepts::Graph G> void tarjan_rec(const G &graph, Vertex v) {
  _tarjan_tree[v].index = index;
  _tarjan_tree[v].low_link = index;
  ++index;
  _stack.push_back(v);
  _tarjan_tree[v].on_stack = true;

  for (auto edge : graph.out_edges(v)) {
    if (_tarjan_tree[edge.v].index == -1) {
      tarjan_rec(graph, edge.v);
      _tarjan_tree[v].low_link =
          std::min(_tarjan_tree[v].low_link, _tarjan_tree[edge.v].low_link);
    } else if (_tarjan_tree[edge.v].on_stack) {
      _tarjan_tree[v].low_link =
          std::min(_tarjan_tree[v].low_link, _tarjan_tree[edge.v].index);
    }
  }
  if (_tarjan_tree[v].low_link == _tarjan_tree[v].index) {
    std::vector<Id> new_scc;

    auto w = _stack.back();
    _stack.pop_back();
    _tarjan_tree[w].on_stack = false;
    new_scc.push_back(w);

    while (w != v) {
      w = _stack.back();
      _stack.pop_back();
      _tarjan_tree[w].on_stack = false;
      new_scc.push_back(w);
    }

    _scc_vector.push_back(new_scc);
  }
}

template <concepts::Graph G> SCCVector visit(const G &graph) {
  _scc_vector.clear();
  _tarjan_tree.clear();
  _stack.clear();
  index = 0;

  for (auto vertex : graph.vertices()) {
    _tarjan_tree[vertex] = TarjanVertex{};
  }

  for (auto vertex : graph.vertices()) {
    if (_tarjan_tree[vertex].index != -1) {
      continue;
    }
    tarjan_rec(graph, vertex);
  }

  return _scc_vector;
}

} // namespace graph::algorithms::tarjan