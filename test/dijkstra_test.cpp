#include "base.hpp"
#include "catch.hpp"
#include "dijkstra.hpp"
#include "list_graph.hpp"

namespace dijsktra_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Dijkstra shortest paths", "[dijsktra]") {
  AdjacencyListGraph<DirectedGraph> g{};

  std::unordered_map<Id, int> weights;

  auto a = g.add_vertex(); // 0
  auto b = g.add_vertex(); // 1
  auto c = g.add_vertex(); // 2
  auto d = g.add_vertex(); // 3
  auto e = g.add_vertex(); // 4
  auto f = g.add_vertex(); // 5

  auto a_to_b = g.add_edge(a, b); // 0->1
  auto a_to_c = g.add_edge(a, c); // 0->2
  auto b_to_c = g.add_edge(b, c); // 1->2
  auto c_to_d = g.add_edge(c, d); // 2->3
  auto c_to_e = g.add_edge(c, e); // 2->4
  auto c_to_f = g.add_edge(c, f); // 2->5
  auto d_to_f = g.add_edge(d, f); // 3->5
  auto e_to_f = g.add_edge(e, f); // 4->5

  /*
    A-->B
    |   |
    |   v
    --->C-->D------v
        --->E------v
        ---------->F
  */

  SECTION("throws on negative edge found") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_b] = -1;

    REQUIRE_THROWS(dijkstra::visit(g, a, weights));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[c_to_f] = 7;

    auto tree = dijkstra::visit(g, a, weights);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 2);
    REQUIRE(tree[e].distance == 2);
    REQUIRE(tree[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = dijkstra::visit(g, a, weights);

    REQUIRE(tree[a].parent == INVALID_VERTEX);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == c);
    REQUIRE(tree[e].parent == c);
    REQUIRE(tree[f].parent == c);
  }

  auto a_to_a = g.add_edge(a, a); // 0->0
  auto d_to_d = g.add_edge(d, d); // 3->3
  auto e_to_c = g.add_edge(e, c); // 4->2

  /*
   <->
    A-->B
    |   |
    |   v  <->
    --->C-->D-------v
        --->E------>F
        <----
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[c_to_f] = 7;
    weights[d_to_d] = 0;
    weights[e_to_c] = 0;

    auto tree = dijkstra::visit(g, a, weights);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 2);
    REQUIRE(tree[e].distance == 2);
    REQUIRE(tree[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights, now with cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = dijkstra::visit(g, a, weights);

    REQUIRE(tree[a].parent == INVALID_VERTEX);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == c);
    REQUIRE(tree[e].parent == c);
    REQUIRE(tree[f].parent == c);
  }
}
} // namespace dijsktra_test