#include "base.hpp"
#include "catch.hpp"
#include "io/graphviz.hpp"
#include "list_graph.hpp"
#include <sstream>
#include <cctype>
#include <string>
#include <map>

namespace graphviz_test {
using namespace graph;
using namespace graph::io;

TEST_CASE("directed graph object is correctly serialized", "[graphviz]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};
  Vertex a = g.add_vertex(); // 0
  Vertex b = g.add_vertex(); // 1
  Vertex c = g.add_vertex(); // 2
  Vertex d = g.add_vertex(); // 3
  Edge a_to_c = g.add_edge(a, c);
  Edge a_to_d = g.add_edge(a, d);
  Edge d_to_c = g.add_edge(d, c);

  SECTION("graph structure is preserved during serialization") {
    const std::string EXPECTED_OUTPUT = "digraph{3;2;1;0;3->2;0->3;0->2;}";
    std::stringstream out;
    graphviz::serialize(out, g);
    std::string s = out.str();
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    REQUIRE(s == EXPECTED_OUTPUT);
  }

  SECTION("vertex properties are serialized correctly") {
    std::unordered_map<int, std::unordered_map<std::string, std::string>> vertex_properties;
    vertex_properties[a] = {{"label", "A"}, {"color", "red"}}; 
    vertex_properties[b] = {{"label", "B"}}; 
    vertex_properties[c] = {{"label", "C"}}; 
    vertex_properties[d] = {{"label", "D"}}; 

    const std::string EXPECTED_OUTPUT = "digraph{3[label=\"D\"];2[label=\"C\"];1[label=\"B\"];0[color=\"red\",label=\"A\"];3->2;0->3;0->2;}";
    std::stringstream out;
    graphviz::serialize(out, g, [&](Vertex v) {return vertex_properties[v];});
    std::string s = out.str();
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    REQUIRE(s == EXPECTED_OUTPUT);
  }

  SECTION("edge properties are serialized correctly") {
    std::unordered_map<int, std::unordered_map<std::string, std::string>> vertex_properties;
    std::unordered_map<int, std::unordered_map<std::string, std::string>> edge_properties;
    vertex_properties[a] = {{"label", "A"}}; 
    edge_properties[a_to_c] = {{"weight", "10"}};
    edge_properties[a_to_d] = {{"weight", "32"}};
    edge_properties[d_to_c] = {{"weight", "5"}, {"foo", "bar"}};

    const std::string EXPECTED_OUTPUT = "digraph{3;2;1;0[label=\"A\"];3->2[foo=\"bar\",weight=\"5\"];0->3[weight=\"32\"];0->2[weight=\"10\"];}";
    std::stringstream out;
    graphviz::serialize(out, g, [&](Vertex v) {return vertex_properties[v];}, [&](Edge e) {return edge_properties[e];});
    std::string s = out.str();
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    REQUIRE(s == EXPECTED_OUTPUT);
  }
}

TEST_CASE("undirected graph object is correctly serialized", "[graphviz]") {
  AdjacencyListGraph<Directedness::UNDIRECTED> g{};
  Vertex a = g.add_vertex(); // 0
  Vertex b = g.add_vertex(); // 1
  Vertex c = g.add_vertex(); // 2
  Vertex d = g.add_vertex(); // 3
  Edge a_to_c = g.add_edge(a, c);
  Edge a_to_c_2 = g.add_edge(a, c);
  Edge a_to_d = g.add_edge(a, d);
  Edge d_to_c = g.add_edge(d, c);

  SECTION("graph structure is preserved during serialization") {
    const std::string EXPECTED_OUTPUT = "graph{3;2;1;0;3--2;0--3;0--2;0--2;}";
    std::stringstream out;
    graphviz::serialize(out, g);
    std::string s = out.str();
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    REQUIRE(s == EXPECTED_OUTPUT);
  }

  SECTION("vertex and edge properties are serialized correctly") {
    std::unordered_map<int, std::unordered_map<std::string, std::string>> vertex_properties;
    std::unordered_map<int, std::unordered_map<std::string, std::string>> edge_properties;
    vertex_properties[a] = {{"label", "A"}}; 
    edge_properties[a_to_c] = {{"weight", "10"}};
    edge_properties[a_to_c_2] = {{"weight", "32"}};
    edge_properties[d_to_c] = {{"weight", "9"}};

    const std::string EXPECTED_OUTPUT = "graph{3;2;1;0[label=\"A\"];3--2[weight=\"9\"];0--3;0--2[weight=\"32\"];0--2[weight=\"10\"];}";
    std::stringstream out;
    graphviz::serialize(out, g, [&](Vertex v) {return vertex_properties[v];}, [&](Edge e) {return edge_properties[e];});
    std::string s = out.str();
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    REQUIRE(s == EXPECTED_OUTPUT);
  }
}

} // namespace graphviz_test
