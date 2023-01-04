#include "algorithms/ford_fulkerson.hpp"
#include "list_graph.hpp"
#include <iostream>

using namespace graph;
using namespace graph::algorithms;

int main() {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<Id, int> weights;

  auto s = g.add_vertex(); // 0
  auto a = g.add_vertex(); // 1
  auto b = g.add_vertex(); // 2
  auto c = g.add_vertex(); // 3
  auto d = g.add_vertex(); // 4
  auto t = g.add_vertex(); // 5

  auto s_to_a = g.add_edge(s, a); // 0->1
  auto s_to_d = g.add_edge(s, d); // 0->4
  auto a_to_b = g.add_edge(a, b); // 1->2
  auto b_to_t = g.add_edge(b, t); // 2->5
  auto c_to_t = g.add_edge(c, t); // 3->5
  auto d_to_b = g.add_edge(d, b); // 4->2
  auto d_to_c = g.add_edge(d, c); // 4->3

  weights[s_to_a] = 8;
  weights[s_to_d] = 3;
  weights[a_to_b] = 9;
  weights[b_to_t] = 2;
  weights[c_to_t] = 5;
  weights[d_to_b] = 7;
  weights[d_to_c] = 4;

  auto result = ford_fulkerson::visit(g, s, t, weights);

  std::cout << result;

  return 0;
}