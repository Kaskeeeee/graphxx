#include "base.hpp"
#include "catch.hpp"
#include "ford_fulkerson.hpp"
#include "list_graph.hpp"
#include <iostream>

namespace ford_fulkerson_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Ford Fulkerson maximum flow", "[ford_fulkerson]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<Id, int> weights;

  // Programiz

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

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
  */

  SECTION("throws on negative edge found") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_b] = -1;

    REQUIRE_THROWS(ford_fulkerson::visit(g, s, t, weights));
  }

  SECTION("finds the maximum possible flow with simple weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto result = ford_fulkerson::visit(g, s, t, weights);

    REQUIRE(result.max_flow == 2);
  }

  SECTION("finds the maximum possible flow") {
    weights[s_to_a] = 8;
    weights[s_to_d] = 3;
    weights[a_to_b] = 9;
    weights[b_to_t] = 2;
    weights[c_to_t] = 5;
    weights[d_to_b] = 7;
    weights[d_to_c] = 4;

    auto result = ford_fulkerson::visit(g, s, t, weights);

    REQUIRE(result.max_flow == 6);
  }
}

} // namespace ford_fulkerson_test