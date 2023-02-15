/**
 * @file This file contains the unit tests for the graphviz serializer
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele
 * Quaresmini, Andrea Cinelli. All rights reserved.
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
#include "io/graphviz.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include <cctype>
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace graphviz_test {
using namespace graphxx;
using namespace graphxx::io;

TEST_CASE(
    "directed list graph object is correctly serialized in graphviz format",
    "[graphviz][list_graph][directed]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphviz_serialize(out, g);
    std::string s = out.str();
    REQUIRE(utils::contains(s, "digraph"));
    REQUIRE(!utils::contains(s, "->"));
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
    graphviz_serialize(out, g);
    std::string s = out.str();

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge =
          std::to_string(source) + "->" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
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
    graphviz_serialize(out, g,
                       [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge =
          std::to_string(source) + "->" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphviz_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge = std::to_string(source) + "->" + std::to_string(target);
      if (edge_properties.contains({source, target})) {
        edge += " [";
        bool comma = false;
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (comma)
            edge += ",";
          else
            comma = true;

          edge += name + "=\"" + value + "\"";
        }
        edge += "]";
      }
      edge += ";";

      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE(
    "directed list graph object is correctly deserialized from graphviz format",
    "[graphviz][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHVIZ_INPUT =
        "digraph { 0; 1; 2; 3; 0->2; 0->3; 3->2; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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

  SECTION("parse simple graph file without forward declaration of nodes") {
    const std::string GRAPHVIZ_INPUT =
        "digraph { 0->2; 0->3; 3->2; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

    REQUIRE(g.num_vertices() == 3);
    REQUIRE(g.num_edges() == 3);
    REQUIRE(g.has_vertex(0));

    enum vertices { a, b, c };
    for (int v = a; v < c; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, b));
    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(c, b));
  }

  SECTION("parse graph with vertex and edges attributes") {
    const std::string GRAPHVIZ_INPUT =
        "digraph { 0 [color=\"red\",label=\"A\"]; 1 [label=\"B\"]; 2 "
        "[label=\"C\"]; 3 [label=\"D\"]; 0->2 [weight=\"10\"]; 0->3 "
        "[weight=\"32\"]; 3->2 [foo=\"bar\",weight=\"5\"]; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT = "graph { 0--2; }";
    std::istringstream istream(GRAPHVIZ_INPUT);

    REQUIRE_THROWS_AS(
        graphviz_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::UndirectedGraphParseException);
  }
}

TEST_CASE("directed list graph object is preserved in serialization and "
          "deserialization in graphviz format",
          "[graphviz][list_graph][directed]") {

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
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphviz_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphviz_deserialize(istream, g2, vertex_properties, edge_properties);

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

TEST_CASE(
    "undirected list graph object is correctly serialized in graphviz format",
    "[graphviz][list_graph][undirected]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphviz_serialize(out, g);
    std::string s = out.str();
    REQUIRE(utils::contains(s, "graph"));
    REQUIRE(!utils::contains(s, "--"));
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
    graphviz_serialize(out, g);
    std::string s = out.str();

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;
    for (auto [source, target] : utils::get_sorted_edges(g)) {
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      std::string edge =
          std::to_string(source) + "--" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
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
    graphviz_serialize(out, g,
                       [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;
    for (auto [source, target] : utils::get_sorted_edges(g)) {
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      std::string edge =
          std::to_string(source) + "--" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{c, a}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, a}] = {{"weight", "32"}};
  edge_properties[{c, d}] = {{"weight", "5"}, {"foo", "bar"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphviz_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge = std::to_string(source) + "--" + std::to_string(target);
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      if (edge_properties.contains({source, target})) {
        edge += " [";
        bool comma = false;
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (comma)
            edge += ",";
          else
            comma = true;

          edge += name + "=\"" + value + "\"";
        }
        edge += "]";
      }
      edge += ";";

      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE("undirected list graph object is correctly deserialized from "
          "graphviz format",
          "[graphviz][list_graph][undirected]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHVIZ_INPUT =
        "graph { 0; 1; 2; 3; 0--2; 0--3; 2--3; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT =
        "graph { 0 [color=\"red\",label=\"A\"]; 1 [label=\"B\"]; 2 "
        "[label=\"C\"]; 3 [label=\"D\"]; 0--2 [weight=\"10\"]; 0--3 "
        "[weight=\"32\"]; 2--3 [foo=\"bar\",weight=\"5\"]; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT = "digraph { 0->2; }";
    std::istringstream istream(GRAPHVIZ_INPUT);

    REQUIRE_THROWS_AS(
        graphviz_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::DirectedGraphParseException);
  }
}

TEST_CASE("undirected list graph object is preserved in serialization and "
          "deserialization in graphviz format",
          "[graphviz][list_graph][undirected]") {

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
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  edge_properties[{c, d}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphviz_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphviz_deserialize(istream, g2, vertex_properties, edge_properties);

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

TEST_CASE(
    "directed matrix graph object is correctly serialized in graphviz format",
    "[graphviz][matrix_graph][directed]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphviz_serialize(out, g);
    std::string s = out.str();
    REQUIRE(utils::contains(s, "digraph"));
    REQUIRE(!utils::contains(s, "->"));
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
    graphviz_serialize(out, g);
    std::string s = out.str();

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge =
          std::to_string(source) + "->" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
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
    graphviz_serialize(out, g,
                       [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge =
          std::to_string(source) + "->" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphviz_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge = std::to_string(source) + "->" + std::to_string(target);
      if (edge_properties.contains({source, target})) {
        edge += " [";
        bool comma = false;
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (comma)
            edge += ",";
          else
            comma = true;

          edge += name + "=\"" + value + "\"";
        }
        edge += "]";
      }
      edge += ";";

      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE("directed matrix graph object is correctly deserialized from "
          "graphviz format",
          "[graphviz][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHVIZ_INPUT =
        "digraph { 0; 1; 2; 3; 0->2; 0->3; 3->2; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT =
        "digraph { 0 [color=\"red\",label=\"A\"]; 1 [label=\"B\"]; 2 "
        "[label=\"C\"]; 3 [label=\"D\"]; 0->2 [weight=\"10\"]; 0->3 "
        "[weight=\"32\"]; 3->2 [foo=\"bar\",weight=\"5\"]; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT = "graph { 0--2; }";
    std::istringstream istream(GRAPHVIZ_INPUT);

    REQUIRE_THROWS_AS(
        graphviz_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::UndirectedGraphParseException);
  }
}

TEST_CASE("directed matrix graph object is preserved in serialization and "
          "deserialization in graphviz format",
          "[graphviz][matrix_graph][directed]") {

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
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphviz_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphviz_deserialize(istream, g2, vertex_properties, edge_properties);

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

TEST_CASE(
    "undirected matrix graph object is correctly serialized in graphviz format",
    "[graphviz][matrix_graph][undirected]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    graphviz_serialize(out, g);
    std::string s = out.str();
    REQUIRE(utils::contains(s, "graph"));
    REQUIRE(!utils::contains(s, "--"));
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
    graphviz_serialize(out, g);
    std::string s = out.str();

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;
    for (auto [source, target] : utils::get_sorted_edges(g)) {
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      std::string edge =
          std::to_string(source) + "--" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
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
    graphviz_serialize(out, g,
                       [&](Vertex<G> v) { return vertex_properties[v]; });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;
    for (auto [source, target] : utils::get_sorted_edges(g)) {
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      std::string edge =
          std::to_string(source) + "--" + std::to_string(target) + ";";
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  edge_properties[{a, c}] = {{"weight", "10"}};
  edge_properties[{a, d}] = {{"weight", "32"}};
  edge_properties[{c, d}] = {{"weight", "5"}, {"foo", "bar"}};

  SECTION("serialize graph with vertex and edge attributes") {
    std::stringstream out;
    graphviz_serialize(
        out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
        [&](Vertex<G> source, Vertex<G> target) {
          return edge_properties[{source, target}];
        });
    std::string s = out.str();

    for (auto v : utils::get_sorted_vertices(g)) {
      std::string node = std::to_string(v);
      if (vertex_properties.contains(v)) {
        node += " [";
        bool comma = false;
        for (const auto &[name, value] : vertex_properties[v]) {
          if (comma)
            node += ",";
          else
            comma = true;

          node += name + "=\"" + value + "\"";
        }
        node += "]";
      }
      node += ";";
      REQUIRE(utils::contains(s, node));
    }

    std::set<std::pair<Vertex<G>, Vertex<G>>> inserted_edges;

    for (auto [source, target] : utils::get_sorted_edges(g)) {
      std::string edge = std::to_string(source) + "--" + std::to_string(target);
      if (inserted_edges.contains({target, source}))
        continue;

      inserted_edges.insert({source, target});

      if (edge_properties.contains({source, target})) {
        edge += " [";
        bool comma = false;
        for (const auto &[name, value] : edge_properties[{source, target}]) {
          if (comma)
            edge += ",";
          else
            comma = true;

          edge += name + "=\"" + value + "\"";
        }
        edge += "]";
      }
      edge += ";";

      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE("undirected matrix graph object is correctly deserialized from "
          "graphviz format",
          "[graphviz][matrix_graph][undirected]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  SECTION("parse simple graph file") {
    const std::string GRAPHVIZ_INPUT =
        "graph { 0; 1; 2; 3; 0--2; 0--3; 2--3; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT =
        "graph { 0 [color=\"red\",label=\"A\"]; 1 [label=\"B\"]; 2 "
        "[label=\"C\"]; 3 [label=\"D\"]; 0--2 [weight=\"10\"]; 0--3 "
        "[weight=\"32\"]; 2--3 [foo=\"bar\",weight=\"5\"]; }";

    std::istringstream istream(GRAPHVIZ_INPUT);
    graphviz_deserialize(istream, g, vertex_properties, edge_properties);

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
    const std::string GRAPHVIZ_INPUT = "digraph { 0->2; }";
    std::istringstream istream(GRAPHVIZ_INPUT);

    REQUIRE_THROWS_AS(
        graphviz_deserialize(istream, g, vertex_properties, edge_properties),
        exceptions::DirectedGraphParseException);
  }
}

TEST_CASE("undirected matrix graph object is preserved in serialization and "
          "deserialization in graphviz format",
          "[graphviz][matrix_graph][undirected]") {

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
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;
  edge_properties[{b, b}] = {{"weight", "10"}};
  edge_properties[{d, c}] = {{"weight", "5"}, {"foo", "bar"}};
  edge_properties[{c, d}] = {{"weight", "5"}, {"foo", "bar"}};

  std::stringstream out;
  graphviz_serialize(
      out, g, [&](Vertex<G> v) { return vertex_properties[v]; },
      [&](Vertex<G> source, Vertex<G> target) {
        return edge_properties[{source, target}];
      });
  std::string s = out.str();

  G g2;
  vertex_properties.clear();
  edge_properties.clear();
  std::istringstream istream(s);
  graphviz_deserialize(istream, g2, vertex_properties, edge_properties);

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

TEST_CASE("parse file in bad format throws a bad graphviz exception",
          "[graphviz]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  std::unordered_map<Vertex<G>, std::unordered_map<std::string, std::string>>
      vertex_properties;
  std::unordered_map<Edge<G>, std::unordered_map<std::string, std::string>,
                     utils::XorTupleHash<Edge<G>>>
      edge_properties;

  const std::string GRAPHVIZ_INPUT = "foo";
  std::istringstream istream(GRAPHVIZ_INPUT);

  REQUIRE_THROWS_AS(
      graphviz_deserialize(istream, g, vertex_properties, edge_properties),
      exceptions::BadGraphvizParseException);

  const std::string GRAPHVIZ_INPUT2 = "{ 0->1; }";
  std::istringstream istream2(GRAPHVIZ_INPUT2);
  REQUIRE_THROWS_AS(
      graphviz_deserialize(istream2, g, vertex_properties, edge_properties),
      exceptions::BadGraphvizParseException);
}

} // namespace graphviz_test
