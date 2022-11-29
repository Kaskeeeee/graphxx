#include "algorithms/tarjan.hpp"
#include <algorithm>
#include <unordered_map>

namespace graph::algorithms::tarjan {

struct TarjanVertex {
  int index = -1;
  int lowlink = -1;
  bool onStack = false;
};

using StackVector = std::vector<Id>;
using TarjanTree = std::unordered_map<Id, TarjanVertex>;

SCCVector _scc_vector;
TarjanTree _tarjan_tree;
StackVector _stack;
int index;

template <concepts::Graph G> SCCVector visit(const G &graph) {
  _scc_vector.clear();
  _tarjan_tree.clear();
  _stack.clear();
  index = 0;

  for (Vertex vertex : _graph.vertices()) {
    _tarjan_tree[vertex] = TarjanVertex{};
  }
  

  for (auto vertex : _graph.vertices()) {
    if (_tarjan_tree[vertex].index == -1) {
      tarjan_rec(graph, vertex);
    }
  }

  return _scc_vector;
}

template <concepts::Graph G>
void tarjan_rec(const G &graph, Vertex v) {
  _tarjan_tree[v].index = index;
  _tarjan_tree[v].lowlink = index;
  ++index;
  _stack.push_back(v);
  _tarjan_tree[v].onStack = true;

  for (auto edge : graph.out_edges(v)) {
    if (_tarjan_tree[edge.target].index == -1) {
      tarjan_rec(graph, edge.target);
      _tarjan_tree[v].lowlink =
          std::min(_tarjan_tree[v].lowlink, _tarjan_tree[edge.target].lowlink);
    } else if (_tarjan_tree[edge.target].onStack) {
      _tarjan_tree[v].lowlink =
          std::min(_tarjan_tree[v].lowlink, _tarjan_tree[edge.target].index);
    }
  }
  if (_tarjan_tree[v].lowlink == _tarjan_tree[v].index) {
    std::vector<Id> newSCC;
    do {
      auto w = _stack.pop_back();
      _tarjan_tree[v].onStack = false;
      newSCC.push_back();
    } while (w != v);
  }
}

} // namespace graph::algorithms::tarjan