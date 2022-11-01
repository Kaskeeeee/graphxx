#include "catch2/catch.hpp"
#include "g_list_digraph.hpp"

namespace digraph_test {

using namespace graph;

TEST_CASE("digraph is built correctly", "[building]") {
  AdjacencyListDigraph g;

  SECTION("empty graph has no vertices or edges") {
    auto vertices = g.vertices();
    REQUIRE(vertices.size() == 0);
    REQUIRE(vertices.empty());

    auto edges = g.edges();
    REQUIRE(edges.size() == 0);
    REQUIRE(edges.empty());
  }

  SECTION("vertices can be inserted and removed") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Vertex z = g.add_vertex();
    REQUIRE_FALSE(g.vertices().empty());
    REQUIRE(g.vertices().size() == 3);
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == v.id || vertex.id == z.id));

    g.remove_vertex(v);
    REQUIRE_FALSE(g.vertices().empty());
    REQUIRE(g.vertices().size() == 2);
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == z.id));

    g.remove_vertex(u);
    g.remove_vertex(z);
    REQUIRE(g.vertices().size() == 0);
    REQUIRE(g.vertices().empty());

    REQUIRE_FALSE(g.remove_vertex(v));
  }

  SECTION("edges can be inserted and removed") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    Edge e2 = g.add_edge(u, u);
    REQUIRE_FALSE(g.edges().empty());
    REQUIRE(g.edges().size() == 2);

    Edge e3 = g.add_edge(v, u);
    Edge e4 = g.add_edge(u, v);
    REQUIRE(g.edges().size() == 4);

    g.remove_edge(e1);
    g.remove_edge(e2);
    g.remove_edge(e3);
    g.remove_edge(e4);
    REQUIRE(g.edges().size() == 0);
    REQUIRE(g.edges().empty());

    REQUIRE_FALSE(g.remove_edge(e4));
  }
}

} // namespace digraph_test
