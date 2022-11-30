#include "base.hpp"
#include "catch.hpp"
#include "list_graph.hpp"
#include "tarjan.hpp"

namespace tarjan_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Tarjan for strongly connected components", "[tarjan]") {
  AdjacencyListGraph<DirectedGraph> g{};

  auto a = g.add_vertex();
  auto b = g.add_vertex();
  auto c = g.add_vertex();
  auto d = g.add_vertex();
  auto e = g.add_vertex();
  auto f = g.add_vertex();

  /*
      A       B       C       D       E       F
      |------->
              |------->
      <---------------|
              <-------<-------|------->
                              <-------|------->
  */

  auto a_to_b = g.add_edge(a, b);
  auto b_to_c = g.add_edge(b, c);
  auto c_to_a = g.add_edge(c, a);
  auto d_to_b = g.add_edge(d, b);
  auto d_to_c = g.add_edge(d, c);
  auto d_to_e = g.add_edge(d, e);
  auto e_to_d = g.add_edge(e, d);
  auto e_to_f = g.add_edge(e, f);

  SECTION("detects strongly connected components") {
    tarjan::SCCVector scc = tarjan::visit(g);

    REQUIRE(scc.size() == 3);
  }
}
} // namespace tarjan_test