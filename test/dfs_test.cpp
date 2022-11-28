#include "algorithms/algorithms_base.hpp"
#include "base.hpp"
#include "catch.hpp"
#include "dfs.hpp"
#include "list_graph.hpp"

#include <algorithm>

namespace dfs_test {
using namespace graph;
using namespace algorithms;

TEST_CASE("DFS Tree correct visited order", "[DFS]") {
  AdjacencyListGraph<GraphType::Directed> g{};

  graph::algorithms::DFS dfs{g};

  auto a = g.add_vertex(); // 0
  auto b = g.add_vertex(); // 1
  auto c = g.add_vertex(); // 2
  auto d = g.add_vertex(); // 3
  auto e = g.add_vertex(); // 4

  auto a_to_b = g.add_edge(a, b); // 0->1
  auto a_to_c = g.add_edge(a, c); // 0->2
  auto a_to_d = g.add_edge(a, d); // 0->3
  auto b_to_c = g.add_edge(b, c); // 1->2
  auto d_to_e = g.add_edge(d, e); // 3->4

  /*
    A--->B--->C
    --------->
    |
    |
    ---->D--->E
  */

  SECTION("check if all nodes were processed") {
    auto tree = dfs.visit(a);

    for (auto vertex : g.vertices()) {
      REQUIRE(tree[vertex].status == VertexStatus::PROCESSED);
    }
  }

  SECTION("check if all parent node are correct") {
    auto tree = dfs.visit(a);

    REQUIRE(tree[a].parent == -1);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == b);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  SECTION("check if discovery and finishing time are correct") {
    auto tree = dfs.visit(a);

    REQUIRE(tree[a].discovery_time == 1);
    REQUIRE(tree[a].finishing_time == 10);
    REQUIRE(tree[d].discovery_time < tree[e].discovery_time);
    REQUIRE(tree[d].finishing_time > tree[e].finishing_time);
    REQUIRE(tree[e].discovery_time == tree[e].finishing_time - 1);
  }

  auto a_to_a = g.add_edge(a, a); // 0->0
  auto b_to_b = g.add_edge(b, b); // 1->1
  auto c_to_b = g.add_edge(c, b); // 2->1

  /*
    A--->B--->C
    |   <->   |
    |    <-----
    --------->
    |
    |
    ---->D--->E
  */

  SECTION("check if all parent node are correct, now with cycles") {
    auto tree = dfs.visit(a);

    REQUIRE(tree[a].parent == -1);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == b);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  SECTION("check if discovery and finishing time are correct") {
    auto tree = dfs.visit(a);

    REQUIRE(tree[a].discovery_time == 1);
    REQUIRE(tree[a].finishing_time == 10);
    REQUIRE(tree[d].discovery_time < tree[e].discovery_time);
    REQUIRE(tree[d].finishing_time > tree[e].finishing_time);
    REQUIRE(tree[e].discovery_time == tree[e].finishing_time - 1);
  }

  SECTION("check if visit with function work properly") {
    std::vector<Vertex> vertices;

    auto tree = dfs.visit(a, [&](Vertex v) { vertices.push_back(v); });

    for (auto vertex : g.vertices()) {
      REQUIRE(std::find(vertices.begin(), vertices.end(), vertex) !=
              vertices.end());
    }
  }
}
} // namespace dfs_test