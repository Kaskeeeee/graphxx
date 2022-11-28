#include "base.hpp"
#include "catch.hpp"
#include "dijkstra.hpp"
#include "list_graph.hpp"

#include <iostream>

namespace dijsktra_test {
using namespace graph;

TEST_CASE("Dijkstra shortest paths", "[dijsktra]") {
  AdjacencyListGraph<GraphType::Directed> g{};

  std::cout << "test" << std::endl;
  algorithms::Dijkstra<AdjacencyListGraph<GraphType::Directed>, int> dijsktra{
      g};

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

    REQUIRE_THROWS(dijsktra.visit(a, weights));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[c_to_f] = 7;

    auto tree = dijsktra.visit(a, weights);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 2);
    REQUIRE(tree[d].distance == 2);
    REQUIRE(tree[e].distance == 2);
    REQUIRE(tree[f].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = dijsktra.visit(a, weights);

    REQUIRE(tree[a].previous_hop == INVALID_VERTEX);
    REQUIRE(tree[b].previous_hop == a);
    REQUIRE(tree[c].previous_hop == a);
    REQUIRE(tree[d].previous_hop == c);
    REQUIRE(tree[e].previous_hop == c);
    REQUIRE(tree[f].previous_hop == c);
  }

  /*
    auto a_to_a = g.add_edge(a, a); // 0->0
    auto d_to_d = g.add_edge(d, d); // 3->3
    auto e_to_c = g.add_edge(e, c); // 4->2
    */

  /*
   <->
    A-->B
    |   |
    |   v  <->
    --->C-->D-------v
        --->E------>F
        <----
  */
  /*

   SECTION("finds the shortest path length with all positive weights, now with "
           "cycles") {
     for (auto edge : g.edges()) {
       weights[edge] = 1;
     }

     weights[c_to_f] = 7;
     weights[d_to_d] = 0;
     weights[e_to_c] = 0;

     auto tree = dijsktra.visit(a, weights);

     REQUIRE(tree[a].distance == 0);
     REQUIRE(tree[b].distance == 1);
     REQUIRE(tree[c].distance == 2);
     REQUIRE(tree[d].distance == 2);
     REQUIRE(tree[e].distance == 2);
     REQUIRE(tree[f].distance == 2);
   }

   SECTION("finds the previous hop with all positive weights, now with cycles")
   { for (auto edge : g.edges()) { weights[edge] = 1;
     }

     auto tree = dijsktra.visit(a, weights);

     REQUIRE(tree[a].previous_hop == INVALID_VERTEX);
     REQUIRE(tree[b].previous_hop == a);
     REQUIRE(tree[c].previous_hop == a);
     REQUIRE(tree[d].previous_hop == c);
     REQUIRE(tree[e].previous_hop == c);
     REQUIRE(tree[f].previous_hop == c);
   }
   */
}
} // namespace dijsktra_test