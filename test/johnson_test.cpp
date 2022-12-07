#include "base.hpp"
#include "catch.hpp"
#include "johnson.hpp"
#include "list_graph.hpp"

namespace johnson_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Johnson shortest paths", "[johnson]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<Id, int> weights;

  auto a = g.add_vertex(); // 0
  auto b = g.add_vertex(); // 1
  auto c = g.add_vertex(); // 2
  auto d = g.add_vertex(); // 3

  auto a_to_c = g.add_edge(a, c); // 0->2
  auto b_to_a = g.add_edge(b, a); // 1->0
  auto b_to_c = g.add_edge(b, c); // 1->2
  auto c_to_d = g.add_edge(c, d); // 2->3
  auto d_to_b = g.add_edge(d, b); // 3->1

  /*
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_c] = -2;

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == -2);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == -1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == -1);
    REQUIRE(tree[a][b].distance == 0);
  }

  SECTION("finds the parent nodes with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_c] = -2;

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE(tree[b][c].parent == a);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  /*
    SECTION("throws on negative cycle found") {
      for (auto edge : g.edges()) {
        weights[edge] = 1;
      }

      weights[c_to_d] = -1;
      weights[d_to_b] = -1;

      REQUIRE_THROWS(floyd_warshall::visit(g, weights));
    }
  */

  auto c_to_a = g.add_edge(c, a); // 2->0
  auto d_to_d = g.add_edge(d, d); // 3->3

  /*
    <--------
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
   <->
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][a].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights, now with cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == b || tree[b][c].parent == a));
    REQUIRE(tree[c][a].parent == c);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }
}
} // namespace johnson_test