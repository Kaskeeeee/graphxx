#include "base.hpp"
#include "catch.hpp"
#include "kruskal.hpp"
#include "list_graph.hpp"

namespace kruskal_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Kruskal minimum spanning tree", "[kruskal]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<Id, int> weights;

  auto a = g.add_vertex();
  auto b = g.add_vertex();
  auto c = g.add_vertex();
  auto d = g.add_vertex();

  /*
      A       B       C       D
      |-------|
              |-------|
      |---------------|-------|
      |-----------------------|
  */

  auto a_to_b = g.add_edge(a, b);
  auto a_to_c = g.add_edge(a, c);
  auto a_to_d = g.add_edge(a, d);
  auto b_to_a = g.add_edge(b, a);
  auto c_to_a = g.add_edge(c, a);
  auto d_to_a = g.add_edge(d, a);

  auto b_to_c = g.add_edge(b, c);

  auto c_to_d = g.add_edge(c, d);

  weights[a_to_b] = 2;
  weights[a_to_c] = 1;
  weights[a_to_d] = 4;
  weights[b_to_a] = 2;
  weights[d_to_a] = 1;
  weights[d_to_a] = 4;
  weights[b_to_c] = 3;
  weights[c_to_d] = 9;

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal::visit(g, weights);

    REQUIRE(vector.size() == 3);
  }
}
} // namespace kruskal_test
