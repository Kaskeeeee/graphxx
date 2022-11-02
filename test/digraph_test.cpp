#include "catch2/catch.hpp"
#include "g_list_digraph.hpp"

namespace digraph_test {

using namespace graph;

TEST_CASE("digraph is built correctly", "[digraph]") {
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

TEST_CASE("digraph manages vertices and edges correctly", "[digraph]") {
  AdjacencyListDigraph g;

  SECTION("creating edges with new vertices adds them to graph") {
    Vertex u = {100};
    Vertex v = {200};
    g.add_edge(u, v);
    REQUIRE(g.edges().size() == 1);
    REQUIRE(g.vertices().size() == 2);
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == v.id));
  }

  SECTION("removing a vertex the connected edges are deleted") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    REQUIRE(g.edges().size() == 1);
    REQUIRE_FALSE(g.edges().empty());
    g.remove_vertex(u);
    REQUIRE(g.edges().size() == 0);
    REQUIRE(g.edges().empty());
    REQUIRE(g.vertices().size() == 1);
  }

  SECTION("the outgoing edges are calculated correctly") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Vertex z = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    Edge e2 = g.add_edge(v, v);
    Edge e3 = g.add_edge(v, z);

    REQUIRE_FALSE(g.out_edges(u).empty());
    REQUIRE(g.out_edges(u).size() == 1);
    REQUIRE_FALSE(g.out_edges(v).empty());
    REQUIRE(g.out_edges(v).size() == 2);
    REQUIRE(g.out_edges(z).empty());
    REQUIRE(g.out_edges(z).size() == 0);
    for (Edge edge : g.out_edges(v))
      REQUIRE((edge.id == e2.id || edge.id == e3.id));
  }

  SECTION("the incoming edges are calculated correctly") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    Edge e2 = g.add_edge(v, v);

    REQUIRE(g.in_edges(u).empty());
    REQUIRE(g.in_edges(u).size() == 0);
    REQUIRE_FALSE(g.in_edges(v).empty());
    REQUIRE(g.in_edges(v).size() == 2);
    for (Edge edge : g.out_edges(v))
      REQUIRE((edge.id == e1.id || edge.id == e2.id));
  }
}

} // namespace digraph_test
