/**
 * @file This file contains the unit tests for the matrix market serializer
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
#include "graph_utils.hpp"
#include "io/matrix_market.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include "utils/string_utils.hpp"

#include <sstream>
#include <string>

namespace matrix_market_test {
using namespace graphxx;
using namespace graphxx::io;

TEST_CASE("directed list graph object is correctly serialized in matrix market "
          "format",
          "[matrix_market][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == "0 0 0");
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
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      REQUIRE(utils::contains(
          s, (std::to_string(source + 1) + " " + std::to_string(target + 1))));
    }
  }

  std::unordered_map<Edge<G>, int, XorTupleHash<Edge<G>>> weights_int;
  weights_int[{a, c}] = 10;
  weights_int[{a, d}] = 32;
  weights_int[{d, c}] = 5;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<int>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_int[e];
      return 1;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate integer general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      int w = weights_int.contains({source, target})
                  ? weights_int[{source, target}]
                  : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, double, XorTupleHash<Edge<G>>> weights_float;
  weights_float[{a, c}] = 10.0;
  weights_float[{a, d}] = 32.5;
  weights_float[{d, c}] = 0.002;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<double>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_float[e];
      return 1.0;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate real general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      double w = weights_float.contains({source, target})
                     ? weights_float[{source, target}]
                     : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE(
    "directed list graph object is correctly deserialized from matrix market "
    "format",
    "[matrix_market][list_graph][directed]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, double>;
  G g;

  SECTION("parse simple graph file") {
    const std::string MM_INPUT = "%%MatrixMarket matrix coordinate pattern "
                                 "general\n4 4 3\n1 3\n1 4\n4 3";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<double>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
  }

  SECTION("parse weighted graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate real general \n4 4 3 \n1 3 10.000000 "
        "\n1 4 32.500000 \n4 3 0.002000";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<double>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(std::get<0>(g.get_attributes(a, c)) == 10.000000);
    REQUIRE(std::get<0>(g.get_attributes(a, d)) == 32.500000);
    REQUIRE(std::get<0>(g.get_attributes(d, c)) == 0.002000);
  }

  SECTION("parse bad file throw an exception") {
    const std::string MM_INPUT = "foo";
    std::istringstream istream(MM_INPUT);
    REQUIRE_THROWS_AS(mm_deserialize<double>(istream, g),
                      exceptions::BadMatrixMarketParseException);
  }
}

TEST_CASE(
    "undirected list graph object is correctly serialized in matrix market "
    "format",
    "[matrix_market][list_graph][undirected]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == "0 0 0");
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
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      REQUIRE(utils::contains(
          s, (std::to_string(source + 1) + " " + std::to_string(target + 1))));
    }
  }

  std::unordered_map<Edge<G>, int, XorTupleHash<Edge<G>>> weights_int;
  weights_int[{a, c}] = 10;
  weights_int[{a, d}] = 32;
  weights_int[{d, c}] = 5;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<int>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_int[e];
      return 1;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate integer general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      int w = weights_int.contains({source, target})
                  ? weights_int[{source, target}]
                  : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, double, XorTupleHash<Edge<G>>> weights_float;
  weights_float[{a, c}] = 10.0;
  weights_float[{a, d}] = 32.5;
  weights_float[{d, c}] = 0.002;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<double>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_float[e];
      return 1.0;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate real general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      double w = weights_float.contains({source, target})
                     ? weights_float[{source, target}]
                     : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE(
    "undirected list graph object is correctly deserialized from matrix market "
    "format",
    "[matrix_market][list_graph][undirected]") {

  using G = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  G g;

  SECTION("parse simple graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate pattern general\n4 "
        "4 6\n1 3\n1 4\n3 1\n3 4\n4 1\n4 3";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<int>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
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

  SECTION("parse weighted graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate integer general\n4 4 6\n1 3 10\n1 4 "
        "32\n3 1 10\n3 4 5\n4 1 32\n4 3 5";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<int>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(std::get<0>(g.get_attributes(c, a)) == 10);
    REQUIRE(std::get<0>(g.get_attributes(d, a)) == 32);
    REQUIRE(std::get<0>(g.get_attributes(d, c)) == 5);
    REQUIRE(std::get<0>(g.get_attributes(c, d)) == 5);
  }

  SECTION("parse bad file throw an exception") {
    const std::string MM_INPUT = "foo";
    std::istringstream istream(MM_INPUT);
    REQUIRE_THROWS_AS(mm_deserialize<double>(istream, g),
                      exceptions::BadMatrixMarketParseException);
  }
}

TEST_CASE(
    "directed matrix graph object is correctly serialized in matrix market "
    "format",
    "[matrix_market][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == "0 0 0");
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
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      REQUIRE(utils::contains(
          s, (std::to_string(source + 1) + " " + std::to_string(target + 1))));
    }
  }

  std::unordered_map<Edge<G>, int, XorTupleHash<Edge<G>>> weights_int;
  weights_int[{a, c}] = 10;
  weights_int[{a, d}] = 32;
  weights_int[{d, c}] = 5;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<int>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_int[e];
      return 1;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate integer general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      int w = weights_int.contains({source, target})
                  ? weights_int[{source, target}]
                  : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, double, XorTupleHash<Edge<G>>> weights_float;
  weights_float[{a, c}] = 10.0;
  weights_float[{a, d}] = 32.5;
  weights_float[{d, c}] = 0.002;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<double>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_float[e];
      return 1.0;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate real general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      double w = weights_float.contains({source, target})
                     ? weights_float[{source, target}]
                     : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE(
    "directed matrix graph object is correctly deserialized from matrix market "
    "format",
    "[matrix_market][matrix_graph][directed]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, double>;
  G g;

  SECTION("parse simple graph file") {
    const std::string MM_INPUT = "%%MatrixMarket matrix coordinate pattern "
                                 "general\n4 4 3\n1 3\n1 4\n4 3";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<double>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
  }

  SECTION("parse weighted graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate real general \n4 4 3 \n1 3 10.000000 "
        "\n1 4 32.500000 \n4 3 0.002000";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<double>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 3);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(std::get<0>(g.get_attributes(a, c)) == 10.000000);
    REQUIRE(std::get<0>(g.get_attributes(a, d)) == 32.500000);
    REQUIRE(std::get<0>(g.get_attributes(d, c)) == 0.002000);
  }

  SECTION("parse bad file throw an exception") {
    const std::string MM_INPUT = "foo";
    std::istringstream istream(MM_INPUT);
    REQUIRE_THROWS_AS(mm_deserialize<double>(istream, g),
                      exceptions::BadMatrixMarketParseException);
  }
}

TEST_CASE(
    "undirected matrix graph object is correctly serialized in matrix market "
    "format",
    "[matrix_market][matrix_graph][undirected]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  G g;

  SECTION("serialize empty graph") {
    std::stringstream out;
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3);
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == "0 0 0");
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
    mm_serialize(out, g);
    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate pattern general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      REQUIRE(utils::contains(
          s, (std::to_string(source + 1) + " " + std::to_string(target + 1))));
    }
  }

  std::unordered_map<Edge<G>, int, XorTupleHash<Edge<G>>> weights_int;
  weights_int[{a, c}] = 10;
  weights_int[{a, d}] = 32;
  weights_int[{d, c}] = 5;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<int>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_int[e];
      return 1;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate integer general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      int w = weights_int.contains({source, target})
                  ? weights_int[{source, target}]
                  : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }

  std::unordered_map<Edge<G>, double, XorTupleHash<Edge<G>>> weights_float;
  weights_float[{a, c}] = 10.0;
  weights_float[{a, d}] = 32.5;
  weights_float[{d, c}] = 0.002;

  SECTION("serialize weighted graph (integer weight)") {
    std::stringstream out;
    mm_serialize<double>(out, g, [&](Edge<G> e) {
      if (weights_int.contains(e))
        return weights_float[e];
      return 1.0;
    });

    std::string s = out.str();
    std::vector<std::string> rows = utils::split(s, "\n");

    REQUIRE(rows.size() == 3 + g.num_edges());
    REQUIRE(rows[0] == "%%MatrixMarket matrix coordinate real general");
    REQUIRE(rows[1] == (std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_vertices()) + " " +
                        std::to_string(g.num_edges())));

    for (auto [source, target] : get_sorted_edges(g)) {
      double w = weights_float.contains({source, target})
                     ? weights_float[{source, target}]
                     : 1;
      std::string edge = std::to_string(source + 1) + " " +
                         std::to_string(target + 1) + " " + std::to_string(w);
      REQUIRE(utils::contains(s, edge));
    }
  }
}

TEST_CASE("undirected matrix graph object is correctly deserialized from "
          "matrix market "
          "format",
          "[matrix_market][matrix_graph][undirected]") {

  using G = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  G g;

  SECTION("parse simple graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate pattern general\n4 "
        "4 6\n1 3\n1 4\n3 1\n3 4\n4 1\n4 3";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<int>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
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

  SECTION("parse weighted graph file") {
    const std::string MM_INPUT =
        "%%MatrixMarket matrix coordinate integer general\n4 4 6\n1 3 10\n1 4 "
        "32\n3 1 10\n3 4 5\n4 1 32\n4 3 5";
    std::istringstream istream(MM_INPUT);
    mm_deserialize<int>(istream, g);

    REQUIRE(g.num_vertices() == 4);
    REQUIRE(g.num_edges() == 6);
    enum vertices { a, b, c, d };
    for (int v = a; v < d; v++) {
      REQUIRE(g.has_vertex(v));
    }

    REQUIRE(g.has_edge(a, c));
    REQUIRE(g.has_edge(a, d));
    REQUIRE(g.has_edge(d, c));
    REQUIRE(std::get<0>(g.get_attributes(c, a)) == 10);
    REQUIRE(std::get<0>(g.get_attributes(d, a)) == 32);
    REQUIRE(std::get<0>(g.get_attributes(d, c)) == 5);
    REQUIRE(std::get<0>(g.get_attributes(c, d)) == 5);
  }

  SECTION("parse bad file throw an exception") {
    const std::string MM_INPUT = "foo";
    std::istringstream istream(MM_INPUT);
    REQUIRE_THROWS_AS(mm_deserialize<double>(istream, g),
                      exceptions::BadMatrixMarketParseException);
  }
}

TEST_CASE("parse simple symmetric matrix market file", "[matrix market]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  G g;

  const std::string MM_INPUT =
      "%%MatrixMarket matrix coordinate pattern symmetric\n4 "
      "4 6\n1 3\n1 4\n3 1\n3 4\n4 1\n4 3";
  std::istringstream istream(MM_INPUT);
  mm_deserialize<int>(istream, g);

  REQUIRE(g.num_vertices() == 8);
  REQUIRE(g.num_edges() == 6);
}

TEST_CASE("parse weighted symmetric matrix market file", "[matrix market]") {
  using G = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  G g;

  const std::string MM_INPUT =
      "%%MatrixMarket matrix coordinate integer symmetric\n4 4 6\n1 3 10\n1 4 "
      "32\n3 1 10\n3 4 5\n4 1 32\n4 3 5";
  std::istringstream istream(MM_INPUT);
  mm_deserialize<int>(istream, g);

  REQUIRE(g.num_vertices() == 8);
  REQUIRE(g.num_edges() == 6);
  enum vertices { a, b, c, d };
  for (int v = a; v < d; v++) {
    REQUIRE(g.has_vertex(v));
  }

  REQUIRE(g.has_edge(a, c));
  REQUIRE(g.has_edge(c, d));
  REQUIRE(g.has_edge(a, d));
  REQUIRE(g.has_edge(d, a));
  REQUIRE(g.has_edge(d, c));
  REQUIRE(g.has_edge(c, d));
  REQUIRE(std::get<0>(g.get_attributes(c, a)) == 10);
  REQUIRE(std::get<0>(g.get_attributes(a, c)) == 10);
  REQUIRE(std::get<0>(g.get_attributes(d, a)) == 32);
  REQUIRE(std::get<0>(g.get_attributes(a, d)) == 32);
  REQUIRE(std::get<0>(g.get_attributes(d, c)) == 5);
  REQUIRE(std::get<0>(g.get_attributes(c, d)) == 5);
}

TEST_CASE("parse file in bad format throws a bad matrix market exception",
          "[matrix market]") {
  using G = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, double>;
  G g;

  const std::string MM_INPUT =
      "%%MatrixMarket matrix coordinate integer bar\nfoo";

  std::istringstream istream(MM_INPUT);
  REQUIRE_THROWS_AS(mm_deserialize<double>(istream, g),
                    exceptions::BadMatrixMarketParseException);
}
} // namespace matrix_market_test