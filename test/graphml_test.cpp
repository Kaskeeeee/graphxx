/**
 * @file
 *
 * @copyright Copyright © 2022 Graphxx. All rights reserved.
 *
 * @license{<blockquote>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * </blockquote>}
 *
 * @author Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea
 * Cinelli
 * @date December, 2022
 * @version v1.0
 */

#include "base.hpp"
#include "catch.hpp"
#include "io/graphml.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include "utils.hpp"
#include "utils/tuple_utils.hpp"

#include <cctype>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>

namespace graphml_test {
using namespace graphxx;
using namespace graphxx::io;

TEST_CASE("directed list graph object is correctly serialized",
          "[graphml][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();
    REQUIRE(utils::contains(s, io::XML_HEADER));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_OPEN));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_CLOSE));

    std::string directedness = "<graph id=\"G\" edgedefault=\"directed\">";
    REQUIRE(utils::contains(s, directedness, false));
  }

  enum vertices { a, b, c, d };
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);

  SECTION("serialize simple graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();

    for (int v = a; v < d; v++) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      for (auto &e : g[v]) {
        std::string edge = "source=\"n" + std::to_string(g.get_source(e)) +
                           "\" target=\"n" + std::to_string(g.get_target(e)) +
                           "\">";
        REQUIRE(utils::contains(s, edge));
      }
    }
  }

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};
  vertex_properties[c] = {{"label", "C"}};
  vertex_properties[d] = {{"label", "D"}};

  std::unordered_map<std::string, std::string> vertex_key_ids;

  SECTION("serialize graph with vertex attributes") {
    std::stringstream out;
    graphml_serialize(out, g,
                      [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {

          if (!vertex_key_ids.contains(name)) {
            vertex_key_ids[name] = std::to_string(key_count++);
          }

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";

          REQUIRE(utils::contains(s, attDecl));
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  std::unordered_map<std::string, std::string> edge_key_ids;

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphml_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {
          if (!vertex_key_ids.contains(name))
            vertex_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";

      if (edge_properties.contains({source, target})) {
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (!edge_key_ids.contains(name))
            edge_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + edge_key_ids[name] +
                                "\" for=\"edge\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef =
              "<data key=\"k" + edge_key_ids[name] + "\">" + value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }
  }
}

TEST_CASE("directed list graph object is correctly deserialized",
          "[graphml][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"directed\"> <node id=\"n0\"> </node> <node "
        "id=\"n1\"> </node> <node id=\"n2\"> </node> <node id=\"n3\"> "
        "</node> <edge id=\"e0\" source=\"n0\" target=\"n2\"> </edge> <edge "
        "id=\"e1\" source=\"n0\" target=\"n3\"> </edge> <edge id=\"e2\" "
        "source=\"n3\" target=\"n2\"> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    REQUIRE(g.has_vertex(0));

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
  }

  SECTION("parse graph with vertex and edges attributes") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <key "
        "id=\"k0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/> "
        "<key id=\"k1\" for=\"node\" attr.name=\"label\" "
        "attr.type=\"string\"/> <key id=\"k2\" for=\"edge\" "
        "attr.name=\"weight\" attr.type=\"string\"/> <key id=\"k3\" "
        "for=\"edge\" attr.name=\"foo\" attr.type=\"string\"/> <graph id=\"G\" "
        "edgedefault=\"directed\"> <node id=\"n0\"> <data "
        "key=\"k0\">red</data> <data key=\"k1\">A</data> </node> <node "
        "id=\"n1\"> <data key=\"k1\">B</data> </node> <node id=\"n2\"> <data "
        "key=\"k1\">C</data> </node> <node id=\"n3\"> <data "
        "key=\"k1\">D</data> </node> <edge id=\"e0\" source=\"n0\" "
        "target=\"n2\"> <data key=\"k2\">10</data> </edge> <edge id=\"e1\" "
        "source=\"n0\" target=\"n3\"> <data key=\"k2\">32</data> </edge> <edge "
        "id=\"e2\" source=\"n3\" target=\"n2\"> <data key=\"k3\">bar</data> "
        "<data key=\"k2\">5</data> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(vertex_properties[a]["label"] == "A");
    REQUIRE(vertex_properties[b]["label"] == "B");
    REQUIRE(vertex_properties[c]["label"] == "C");
    REQUIRE(vertex_properties[d]["label"] == "D");
    REQUIRE(vertex_properties[a]["color"] == "red");
    REQUIRE(edge_properties[{a, c}]["weight"] == "10");
    REQUIRE(edge_properties[{a, d}]["weight"] == "32");
    REQUIRE(edge_properties[{d, c}]["weight"] == "5");
    REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
  }

  SECTION("parse undirected graph into a directed graph throw an exception") {
    std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"undirected\"> </graph> </graphml>";
    std::istringstream istream(GRAPHML_INPUT);

    REQUIRE_THROWS_AS(
        graphml_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::DirectedGraphParseException);
  }
}

TEST_CASE("directed list graph object is preserved in serialization and "
          "deserialization",
          "[graphml][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;
  enum vertices { a, b, c, d };
  g.add_vertex(3);
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);
  g.add_edge(b, b);

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphml_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphml_deserialize(istream, g2, vertex_properties, edge_properties);

  REQUIRE(g2.num_vertices() == 4);
  REQUIRE(g2.num_edges() == 4);

  for (int v = a; v < d; v++) {
    REQUIRE(g2.has_vertex(v));
  }

  REQUIRE(g2.has_edge(a, c));
  REQUIRE(g2.has_edge(a, d));
  REQUIRE(g2.has_edge(d, c));
  REQUIRE(g2.has_edge(b, b));
  REQUIRE(vertex_properties[a]["label"] == "A");
  REQUIRE(vertex_properties[b]["label"] == "B");
  REQUIRE(vertex_properties[a]["color"] == "red");
  REQUIRE(edge_properties[{b, b}]["weight"] == "10");
  REQUIRE(edge_properties[{d, c}]["weight"] == "5");
  REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
}

TEST_CASE("undirected list graph object is correctly serialized",
          "[graphml][list_graph][undirected]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();
    REQUIRE(utils::contains(s, io::XML_HEADER));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_OPEN));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_CLOSE));

    std::string directedness = "<graph id=\"G\" edgedefault=\"undirected\">";
    REQUIRE(utils::contains(s, directedness, false));
  }

  enum vertices { a, b, c, d };
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);

  SECTION("serialize simple graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();

    for (int v = a; v < d; v++) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      for (auto &e : g[v]) {
        std::string edge = "source=\"n" + std::to_string(g.get_source(e)) +
                           "\" target=\"n" + std::to_string(g.get_target(e)) +
                           "\">";
        REQUIRE(utils::contains(s, edge));
      }
    }
  }

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};
  vertex_properties[c] = {{"label", "C"}};
  vertex_properties[d] = {{"label", "D"}};

  std::unordered_map<std::string, std::string> vertex_key_ids;

  SECTION("serialize graph with vertex attributes") {
    std::stringstream out;
    graphml_serialize(out, g,
                      [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {

          if (!vertex_key_ids.contains(name)) {
            vertex_key_ids[name] = std::to_string(key_count++);
          }

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";

          REQUIRE(utils::contains(s, attDecl));
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  std::unordered_map<std::string, std::string> edge_key_ids;

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphml_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {
          if (!vertex_key_ids.contains(name))
            vertex_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";

      if (edge_properties.contains({source, target})) {
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (!edge_key_ids.contains(name))
            edge_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + edge_key_ids[name] +
                                "\" for=\"edge\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef =
              "<data key=\"k" + edge_key_ids[name] + "\">" + value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }
  }
}

TEST_CASE("undirected list graph object is correctly deserialized",
          "[graphml][list_graph][undirected]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"undirected\"> <node id=\"n0\"> </node> <node "
        "id=\"n1\"> </node> <node id=\"n2\"> </node> <node id=\"n3\"> "
        "</node> <edge id=\"e0\" source=\"n0\" target=\"n2\"> </edge> <edge "
        "id=\"e1\" source=\"n0\" target=\"n3\"> </edge> <edge id=\"e2\" "
        "source=\"n3\" target=\"n2\"> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
    REQUIRE(g.has_vertex(0));

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(g.has_edge(c, a));
    REQUIRE(g.has_edge(d, a));
    REQUIRE(g.has_edge(c, d));
  }

  SECTION("parse graph with vertex and edges attributes") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <key "
        "id=\"k0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/> "
        "<key id=\"k1\" for=\"node\" attr.name=\"label\" "
        "attr.type=\"string\"/> <key id=\"k2\" for=\"edge\" "
        "attr.name=\"weight\" attr.type=\"string\"/> <key id=\"k3\" "
        "for=\"edge\" attr.name=\"foo\" attr.type=\"string\"/> <graph id=\"G\" "
        "edgedefault=\"undirected\"> <node id=\"n0\"> <data "
        "key=\"k0\">red</data> <data key=\"k1\">A</data> </node> <node "
        "id=\"n1\"> <data key=\"k1\">B</data> </node> <node id=\"n2\"> <data "
        "key=\"k1\">C</data> </node> <node id=\"n3\"> <data "
        "key=\"k1\">D</data> </node> <edge id=\"e0\" source=\"n0\" "
        "target=\"n2\"> <data key=\"k2\">10</data> </edge> <edge id=\"e1\" "
        "source=\"n0\" target=\"n3\"> <data key=\"k2\">32</data> </edge> <edge "
        "id=\"e2\" source=\"n3\" target=\"n2\"> <data key=\"k3\">bar</data> "
        "<data key=\"k2\">5</data> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(vertex_properties[a]["label"] == "A");
    REQUIRE(vertex_properties[b]["label"] == "B");
    REQUIRE(vertex_properties[c]["label"] == "C");
    REQUIRE(vertex_properties[d]["label"] == "D");
    REQUIRE(vertex_properties[a]["color"] == "red");
    REQUIRE(edge_properties[{c, a}]["weight"] == "10");
    REQUIRE(edge_properties[{d, a}]["weight"] == "32");
    REQUIRE(edge_properties[{d, c}]["weight"] == "5");
    REQUIRE(edge_properties[{c, d}]["foo"] == "bar");
  }

  SECTION("parse undirected graph into a directed graph throw an exception") {
    std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"directed\"> </graph> </graphml>";
    std::istringstream istream(GRAPHML_INPUT);

    REQUIRE_THROWS_AS(
        graphml_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::UndirectedGraphParseException);
  }
}

TEST_CASE("undirected list graph object is preserved in serialization and "
          "deserialization",
          "[graphml][list_graph][undirected]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  enum vertices { a, b, c, d };
  g.add_vertex(3);
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);
  g.add_edge(b, b);

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphml_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphml_deserialize(istream, g2, vertex_properties, edge_properties);

  REQUIRE(g2.num_vertices() == 4);
  REQUIRE(g2.num_edges() == 7);

  for (int v = a; v < d; v++) {
    REQUIRE(g2.has_vertex(v));
  }

  REQUIRE(g2.has_edge(a, c));
  REQUIRE(g2.has_edge(c, a));
  REQUIRE(g2.has_edge(d, a));
  REQUIRE(g2.has_edge(d, c));
  REQUIRE(g2.has_edge(b, b));
  REQUIRE(vertex_properties[a]["label"] == "A");
  REQUIRE(vertex_properties[b]["label"] == "B");
  REQUIRE(vertex_properties[a]["color"] == "red");
  REQUIRE(edge_properties[{b, b}]["weight"] == "10");
  REQUIRE(edge_properties[{c, d}]["weight"] == "5");
  REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
}

TEST_CASE("directed matrix graph object is correctly serialized",
          "[graphml][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();
    REQUIRE(utils::contains(s, io::XML_HEADER));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_OPEN));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_CLOSE));

    std::string directedness = "<graph id=\"G\" edgedefault=\"directed\">";
    REQUIRE(utils::contains(s, directedness, false));
  }

  enum vertices { a, b, c, d };
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);

  SECTION("serialize simple graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();

    for (int v = a; v < d; v++) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      for (auto &e : g[v]) {
        std::string edge = "source=\"n" + std::to_string(g.get_source(e)) +
                           "\" target=\"n" + std::to_string(g.get_target(e)) +
                           "\">";
        REQUIRE(utils::contains(s, edge));
      }
    }
  }

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};
  vertex_properties[c] = {{"label", "C"}};
  vertex_properties[d] = {{"label", "D"}};

  std::unordered_map<std::string, std::string> vertex_key_ids;

  SECTION("serialize graph with vertex attributes") {
    std::stringstream out;
    graphml_serialize(out, g,
                      [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {

          if (!vertex_key_ids.contains(name)) {
            vertex_key_ids[name] = std::to_string(key_count++);
          }

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";

          REQUIRE(utils::contains(s, attDecl));
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  std::unordered_map<std::string, std::string> edge_key_ids;

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphml_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {
          if (!vertex_key_ids.contains(name))
            vertex_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";

      if (edge_properties.contains({source, target})) {
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (!edge_key_ids.contains(name))
            edge_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + edge_key_ids[name] +
                                "\" for=\"edge\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef =
              "<data key=\"k" + edge_key_ids[name] + "\">" + value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }
  }
}

TEST_CASE("directed matrix graph object is correctly deserialized",
          "[graphml][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"directed\"> <node id=\"n0\"> </node> <node "
        "id=\"n1\"> </node> <node id=\"n2\"> </node> <node id=\"n3\"> "
        "</node> <edge id=\"e0\" source=\"n0\" target=\"n2\"> </edge> <edge "
        "id=\"e1\" source=\"n0\" target=\"n3\"> </edge> <edge id=\"e2\" "
        "source=\"n3\" target=\"n2\"> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    REQUIRE(g.has_vertex(0));

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
  }

  SECTION("parse graph with vertex and edges attributes") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <key "
        "id=\"k0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/> "
        "<key id=\"k1\" for=\"node\" attr.name=\"label\" "
        "attr.type=\"string\"/> <key id=\"k2\" for=\"edge\" "
        "attr.name=\"weight\" attr.type=\"string\"/> <key id=\"k3\" "
        "for=\"edge\" attr.name=\"foo\" attr.type=\"string\"/> <graph id=\"G\" "
        "edgedefault=\"directed\"> <node id=\"n0\"> <data "
        "key=\"k0\">red</data> <data key=\"k1\">A</data> </node> <node "
        "id=\"n1\"> <data key=\"k1\">B</data> </node> <node id=\"n2\"> <data "
        "key=\"k1\">C</data> </node> <node id=\"n3\"> <data "
        "key=\"k1\">D</data> </node> <edge id=\"e0\" source=\"n0\" "
        "target=\"n2\"> <data key=\"k2\">10</data> </edge> <edge id=\"e1\" "
        "source=\"n0\" target=\"n3\"> <data key=\"k2\">32</data> </edge> <edge "
        "id=\"e2\" source=\"n3\" target=\"n2\"> <data key=\"k3\">bar</data> "
        "<data key=\"k2\">5</data> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(vertex_properties[a]["label"] == "A");
    REQUIRE(vertex_properties[b]["label"] == "B");
    REQUIRE(vertex_properties[c]["label"] == "C");
    REQUIRE(vertex_properties[d]["label"] == "D");
    REQUIRE(vertex_properties[a]["color"] == "red");
    REQUIRE(edge_properties[{a, c}]["weight"] == "10");
    REQUIRE(edge_properties[{a, d}]["weight"] == "32");
    REQUIRE(edge_properties[{d, c}]["weight"] == "5");
    REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
  }

  SECTION("parse undirected graph into a directed graph throw an exception") {
    std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"undirected\"> </graph> </graphml>";
    std::istringstream istream(GRAPHML_INPUT);

    REQUIRE_THROWS_AS(
        graphml_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::DirectedGraphParseException);
  }
}

TEST_CASE("directed matrix graph object is preserved in serialization and "
          "deserialization",
          "[graphml][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;
  enum vertices { a, b, c, d };
  g.add_vertex(3);
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);
  g.add_edge(b, b);

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphml_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphml_deserialize(istream, g2, vertex_properties, edge_properties);

  REQUIRE(g2.num_vertices() == 4);
  REQUIRE(g2.num_edges() == 4);

  for (int v = a; v < d; v++) {
    REQUIRE(g2.has_vertex(v));
  }

  REQUIRE(g2.has_edge(a, c));
  REQUIRE(g2.has_edge(a, d));
  REQUIRE(g2.has_edge(d, c));
  REQUIRE(g2.has_edge(b, b));
  REQUIRE(vertex_properties[a]["label"] == "A");
  REQUIRE(vertex_properties[b]["label"] == "B");
  REQUIRE(vertex_properties[a]["color"] == "red");
  REQUIRE(edge_properties[{b, b}]["weight"] == "10");
  REQUIRE(edge_properties[{d, c}]["weight"] == "5");
  REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
}

TEST_CASE("undirected matrix graph object is correctly serialized",
          "[graphml][matrix_graph][undirected]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();
    REQUIRE(utils::contains(s, io::XML_HEADER));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_OPEN));
    REQUIRE(utils::contains(s, io::GRAPHML_ROOT_CLOSE));

    std::string directedness = "<graph id=\"G\" edgedefault=\"undirected\">";
    REQUIRE(utils::contains(s, directedness, false));
  }

  enum vertices { a, b, c, d };
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_vertex();
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);

  SECTION("serialize simple graph") {
    std::stringstream out;
    graphml_serialize(out, g);

    std::string s = out.str();

    for (int v = a; v < d; v++) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      for (auto &e : g[v]) {
        std::string edge = "source=\"n" + std::to_string(g.get_source(e)) +
                           "\" target=\"n" + std::to_string(g.get_target(e)) +
                           "\">";
        REQUIRE(utils::contains(s, edge));
      }
    }
  }

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};
  vertex_properties[c] = {{"label", "C"}};
  vertex_properties[d] = {{"label", "D"}};

  std::unordered_map<std::string, std::string> vertex_key_ids;

  SECTION("serialize graph with vertex attributes") {
    std::stringstream out;
    graphml_serialize(out, g,
                      [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {

          if (!vertex_key_ids.contains(name)) {
            vertex_key_ids[name] = std::to_string(key_count++);
          }

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";

          REQUIRE(utils::contains(s, attDecl));
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  std::unordered_map<std::string, std::string> edge_key_ids;

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphml_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    int key_count = 0;
    for (auto v : get_sorted_vertices(g)) {
      std::string node = "<node id=\"n" + std::to_string(v) + "\">";
      REQUIRE(utils::contains(s, node));

      if (vertex_properties.contains(v)) {
        for (const auto &[name, value] : vertex_properties[v]) {
          if (!vertex_key_ids.contains(name))
            vertex_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + vertex_key_ids[name] +
                                "\" for=\"node\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef = "<data key=\"k" + vertex_key_ids[name] + "\">" +
                               value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }

    int edge_count = 0;
    for (auto [source, target] : get_sorted_edges(g)) {
      std::string edge = "<edge id=\"e" + std::to_string(edge_count++) +
                         "\" source=\"n" + std::to_string(source) +
                         "\" target=\"n" + std::to_string(target) + "\">";

      if (edge_properties.contains({source, target})) {
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (!edge_key_ids.contains(name))
            edge_key_ids[name] = std::to_string(key_count++);

          std::string attDecl = "<key id=\"k" + edge_key_ids[name] +
                                "\" for=\"edge\" attr.name=\"" + name +
                                "\" attr.type=\"string\"/>";
          std::string attDef =
              "<data key=\"k" + edge_key_ids[name] + "\">" + value + "</data>";

          REQUIRE(utils::contains(s, attDecl));
          REQUIRE(utils::contains(s, attDef));
        }
      }
    }
  }
}

TEST_CASE("undirected matrix graph object is correctly deserialized",
          "[graphml][matrix_graph][undirected]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"undirected\"> <node id=\"n0\"> </node> <node "
        "id=\"n1\"> </node> <node id=\"n2\"> </node> <node id=\"n3\"> "
        "</node> <edge id=\"e0\" source=\"n0\" target=\"n2\"> </edge> <edge "
        "id=\"e1\" source=\"n0\" target=\"n3\"> </edge> <edge id=\"e2\" "
        "source=\"n3\" target=\"n2\"> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
    REQUIRE(g.has_vertex(0));

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(g.has_edge(c, a));
    REQUIRE(g.has_edge(d, a));
    REQUIRE(g.has_edge(c, d));
  }

  SECTION("parse graph with vertex and edges attributes") {
    const std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <key "
        "id=\"k0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/> "
        "<key id=\"k1\" for=\"node\" attr.name=\"label\" "
        "attr.type=\"string\"/> <key id=\"k2\" for=\"edge\" "
        "attr.name=\"weight\" attr.type=\"string\"/> <key id=\"k3\" "
        "for=\"edge\" attr.name=\"foo\" attr.type=\"string\"/> <graph id=\"G\" "
        "edgedefault=\"undirected\"> <node id=\"n0\"> <data "
        "key=\"k0\">red</data> <data key=\"k1\">A</data> </node> <node "
        "id=\"n1\"> <data key=\"k1\">B</data> </node> <node id=\"n2\"> <data "
        "key=\"k1\">C</data> </node> <node id=\"n3\"> <data "
        "key=\"k1\">D</data> </node> <edge id=\"e0\" source=\"n0\" "
        "target=\"n2\"> <data key=\"k2\">10</data> </edge> <edge id=\"e1\" "
        "source=\"n0\" target=\"n3\"> <data key=\"k2\">32</data> </edge> <edge "
        "id=\"e2\" source=\"n3\" target=\"n2\"> <data key=\"k3\">bar</data> "
        "<data key=\"k2\">5</data> </edge> </graph> </graphml>";

    std::istringstream istream(GRAPHML_INPUT);
    graphml_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);

    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(vertex_properties[a]["label"] == "A");
    REQUIRE(vertex_properties[b]["label"] == "B");
    REQUIRE(vertex_properties[c]["label"] == "C");
    REQUIRE(vertex_properties[d]["label"] == "D");
    REQUIRE(vertex_properties[a]["color"] == "red");
    REQUIRE(edge_properties[{c, a}]["weight"] == "10");
    REQUIRE(edge_properties[{d, a}]["weight"] == "32");
    REQUIRE(edge_properties[{d, c}]["weight"] == "5");
    REQUIRE(edge_properties[{c, d}]["foo"] == "bar");
  }

  SECTION("parse undirected graph into a directed graph throw an exception") {
    std::string GRAPHML_INPUT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <graphml "
        "xmlns=\"http://graphml.graphdrawing.org/xmlns\" "
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
        "xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\"> <graph "
        "id=\"G\" edgedefault=\"directed\"> </graph> </graphml>";
    std::istringstream istream(GRAPHML_INPUT);

    REQUIRE_THROWS_AS(
        graphml_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::UndirectedGraphParseException);
  }
}

TEST_CASE("undirected matrix graph object is preserved in serialization and "
          "deserialization",
          "[graphml][matrix_graph][undirected]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  enum vertices { a, b, c, d };
  g.add_vertex(3);
  g.add_edge(a, c);
  g.add_edge(a, d);
  g.add_edge(d, c);
  g.add_edge(b, b);

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  vertex_properties[a] = {{"label", "A"}, {"color", "red"}};
  vertex_properties[b] = {{"label", "B"}};

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     xor_tuple_hash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphml_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphml_deserialize(istream, g2, vertex_properties, edge_properties);

  REQUIRE(g2.num_vertices() == 4);
  REQUIRE(g2.num_edges() == 7);

  for (int v = a; v < d; v++) {
    REQUIRE(g2.has_vertex(v));
  }

  REQUIRE(g2.has_edge(a, c));
  REQUIRE(g2.has_edge(c, a));
  REQUIRE(g2.has_edge(d, a));
  REQUIRE(g2.has_edge(d, c));
  REQUIRE(g2.has_edge(b, b));
  REQUIRE(vertex_properties[a]["label"] == "A");
  REQUIRE(vertex_properties[b]["label"] == "B");
  REQUIRE(vertex_properties[a]["color"] == "red");
  REQUIRE(edge_properties[{b, b}]["weight"] == "10");
  REQUIRE(edge_properties[{c, d}]["weight"] == "5");
  REQUIRE(edge_properties[{d, c}]["foo"] == "bar");
}

} // namespace graphml_test
