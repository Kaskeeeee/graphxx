#include "algorithms/tarjan.hpp"
#include <algorithm>
#include <unordered_map>

namespace graph::algorithms::tarjan {

SCCVector _scc_vector;
TarjanTree _tarjan_tree;
StackVector _stack;
int index;

template <concepts::Graph G> SCCVector visit(const G &graph) {
  _scc_vector.clear();
  _tarjan_tree.clear();
  _stack.clear();
  index = 0;

  for (Vertex vertex : graph.vertices()) {
    _tarjan_tree[vertex] = TarjanVertex{};
  }

  for (auto vertex : graph.vertices()) {
    if (_tarjan_tree[vertex].index == -1) {
      tarjan_rec(graph, vertex);
    }
  }

  return _scc_vector;
}

template <concepts::Graph G> void tarjan_rec(const G &graph, Vertex v) {
  _tarjan_tree[v].index = index;
  _tarjan_tree[v].low_link = index;
  ++index;
  _stack.push_back(v);
  _tarjan_tree[v].on_stack = true;

  for (auto edge : graph.out_edges(v)) {
    if (_tarjan_tree[edge.target].index == -1) {
      tarjan_rec(graph, edge.target);
      _tarjan_tree[v].low_link = std::min(_tarjan_tree[v].low_link,
                                          _tarjan_tree[edge.target].low_link);
    } else if (_tarjan_tree[edge.target].on_stack) {
      _tarjan_tree[v].low_link =
          std::min(_tarjan_tree[v].low_link, _tarjan_tree[edge.target].index);
    }
  }
  if (_tarjan_tree[v].low_link == _tarjan_tree[v].index) {
    std::vector<Id> new_scc;

    do {
      auto w = _stack.pop_back();
      _tarjan_tree[v].on_stack = false;
      new_scc.push_back();
    } while (w != v);
  }
}

} // namespace graph::algorithms::tarjan