#include "algorithms/algorithms_base.hpp"
#include "base.hpp"
#include "bfs.hpp"
#include "catch.hpp"
#include "list_graph.hpp"

#include <algorithm>

namespace bfs_test {
using namespace graph;
using namespace algorithms;

TEST_CASE("BFS Tree correct visited order", "[BFS]") {
  AdjacencyListGraph<GraphType::Directed> g{};

  graph::algorithms::BFS bfs{g};

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
    auto tree = bfs.visit(a);

    for (auto vertex : g.vertices()) {
      REQUIRE(tree[vertex].status == VertexStatus::PROCESSED);
    }
  }

  SECTION("check if all the distances from the source are correct") {
    auto tree = bfs.visit(a);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 2);
  }

  SECTION("check if all parent node are correct") {
    auto tree = bfs.visit(a);

    REQUIRE(tree[a].parent == -1);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  auto a_to_a = g.add_edge(a, a); // 0->0
  auto b_to_b = g.add_edge(b, b); // 1->1
  auto c_to_b = g.add_edge(c, b); // 2->1

  /*
   <->
    A--->B--->C
    |   <->   |
    |    <-----
    --------->
    |
    |
    ---->D--->E
  */

  SECTION("check if all the distances from the source are correct, now with "
          "cycles") {
    auto tree = bfs.visit(a);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 2);
  }

  SECTION("check if all parent node are correct, now with cycles") {
    auto tree = bfs.visit(a);

    REQUIRE(tree[a].parent == -1);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == a);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  SECTION("check if visit with function work properly") {
    std::vector<Vertex> vertices;

    auto tree = bfs.visit(a, [&](Vertex v) { vertices.push_back(v); });

    for (auto vertex : g.vertices()) {
      REQUIRE(std::find(vertices.begin(), vertices.end(), vertex) !=
              vertices.end());
    }
  }
}
} // namespace bfs_test