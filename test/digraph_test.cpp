#include "catch.hpp"
#include "list_graph.hpp"

#include <algorithm>
#include <vector>

namespace digraph_test {

using namespace graph;

TEST_CASE("digraph is built correctly", "[digraph]") {
  AdjacencyListGraph<DirectedGraph> g;

  SECTION("empty graph has no vertices or edges") {
    auto vertices = g.vertices();
    REQUIRE(vertices.empty());

    auto edges = g.edges();
    REQUIRE(edges.empty());
  }

  SECTION("vertices can be inserted and removed") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Vertex z = g.add_vertex();
    REQUIRE_FALSE(g.vertices().empty());
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == v.id || vertex.id == z.id));

    g.remove_vertex(v);
    REQUIRE_FALSE(g.vertices().empty());
    for (Vertex vertex : g.vertices())
      REQUIRE((vertex.id == u.id || vertex.id == z.id));

    g.remove_vertex(u);
    g.remove_vertex(z);
    REQUIRE(g.vertices().empty());

    REQUIRE_THROWS(g.remove_vertex(v));
  }

  SECTION("edges can be inserted and removed") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    Edge e2 = g.add_edge(u, u);
    REQUIRE_FALSE(g.edges().empty());

    Edge e3 = g.add_edge(v, u);
    Edge e4 = g.add_edge(u, v);

    g.remove_edge(e1);
    g.remove_edge(e2);
    g.remove_edge(e3);
    g.remove_edge(e4);
    REQUIRE(g.edges().empty());

    REQUIRE_THROWS(g.remove_edge(e4));
  }
}

TEST_CASE("digraph manages vertices and edges correctly", "[digraph]") {
  AdjacencyListGraph<DirectedGraph> g;

  SECTION("removing a vertex will remove also the connected edges") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    g.add_edge(u, v);
    REQUIRE_FALSE(g.edges().empty());
    g.remove_vertex(u);
    REQUIRE(g.edges().empty());
  }

  SECTION("the outgoing edges are calculated correctly") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Vertex z = g.add_vertex();
    g.add_edge(u, v);
    Edge e2 = g.add_edge(v, v);
    Edge e3 = g.add_edge(v, z);

    REQUIRE_FALSE(g.out_edges(u).empty());
    REQUIRE_FALSE(g.out_edges(v).empty());
    REQUIRE(g.out_edges(z).empty());

    std::vector<Edge> out_edges{g.out_edges(v).begin(), g.out_edges(v).end()};
    std::sort(out_edges.begin(), out_edges.end());

    REQUIRE(out_edges[0] == e2.id);
    REQUIRE(out_edges[1] == e3.id);
  }

  SECTION("the incoming edges are calculated correctly") {
    Vertex u = g.add_vertex();
    Vertex v = g.add_vertex();
    Edge e1 = g.add_edge(u, v);
    Edge e2 = g.add_edge(v, v);

    REQUIRE(g.in_edges(u).empty());
    REQUIRE_FALSE(g.in_edges(v).empty());

    std::vector<Edge> in_edges{g.in_edges(v).begin(), g.in_edges(v).end()};
    std::sort(in_edges.begin(), in_edges.end());

    REQUIRE(in_edges[0] == e1.id);
    REQUIRE(in_edges[1] == e2.id);
  }
}

} // namespace digraph_test
