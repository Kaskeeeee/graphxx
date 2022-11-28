#include "algorithms/tarjan.hpp"
#include <algorithm>

namespace graph::algorithms {

template <concepts::Graph G> Tarjan<G>::Tarjan(G &g) : _graph{g} {};

template <concepts::Graph G> void Tarjan<G>::init() {
  _scc_vector.clear();
  _tarjan_tree.clear();
  _stack.clear();

  for (Vertex vertex : _graph.vertices()) {
    _tarjan_tree[vertex] = TarjanVertex{};
  }

  index = 0;
}

template <concepts::Graph G> Tarjan<G>::SCCVector Tarjan<G>::visit() {
  init();

  for (auto vertex : _graph.vertices()) {
    if (_tarjan_tree[vertex].index == -1) {
      tarjan_rec(vertex);
    }
  }

  return _scc_vector;
}

template <concepts::Graph G> void Tarjan<G>::tarjan_rec(Vertex v) {
  _tarjan_tree[v].index = index;
  _tarjan_tree[v].lowlink = index;
  ++index;
  _stack.push_back(v);
  _tarjan_tree[v].onStack = true;

  for (auto edge : graph.out_edges(v)) {
    if (_tarjan_tree[edge.target].index == -1) {
      tarjan_rec(edge.target);
      _tarjan_tree[v].lowlink = std::min(_tarjan_tree[v].lowlink, _tarjan_tree[edge.target].lowlink);
    } else if(_tarjan_tree[edge.target].onStack){
        _tarjan_tree[v].lowlink = std::min(_tarjan_tree[v].lowlink, _tarjan_tree[edge.target].index);
    }
  }
  if (_tarjan_tree[v].lowlink == _tarjan_tree[v].index){
    std::vector<Id> newSCC;
    do{
        auto w = _stack.pop_back();
        _tarjan_tree[v].onStack = false;
        newSCC.push_back();
    } while(w != v);
  }
}

} // namespace graph::algorithms