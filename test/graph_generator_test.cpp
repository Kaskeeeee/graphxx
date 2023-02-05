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
#include "generators/graph_generator.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

namespace graph_generator_test {
using namespace graphxx;

TEST_CASE("generate directed list graph", "[generator][list_graph][directed]") {
  AdjacencyListGraph<unsigned long, Directedness::DIRECTED> g{};
  GraphGenerator generator;

  const int TOTAL_VERTICES = 15;
  const int TOTAL_EDGES = 20;

  SECTION("generates input limited nodes") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_vertices() == TOTAL_VERTICES);
  }

  SECTION("generates input limited edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_edges() == TOTAL_EDGES);
  }

  SECTION("early returns if exceeds max out degree") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, 1);
    REQUIRE(g.num_edges() == TOTAL_VERTICES);
  }

  SECTION("early returns if there is not other candidate for a new edge when "
          "using no self edges mode") {
    generator.generate_random_graph(g, 1, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("chooses another target id with no self edges mode") {
    generator.generate_random_graph(g, 2, 100, -1, false);

    for (auto edgelist : g) {
      for (auto edge : edgelist) {
        REQUIRE(g.get_source(edge) != g.get_target(edge));
      }
    }
  }

  const int MAX_WEIGHT = 10;
  const int MIN_WEIGHT = 0;

  SECTION("generates weights within bounds") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MIN_WEIGHT, MAX_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION("swaps min and max weight if in bad order") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION(
      "swaps min and max weight if in bad order with fixed seed generator") {
    GraphGenerator fixed_seed_gen{0xdeadbeef};

    fixed_seed_gen.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights =
        fixed_seed_gen.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }
}

TEST_CASE("generate undirected list graph",
          "[generator][list_graph][undirected]") {
  AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED> g{};
  GraphGenerator generator;

  const int TOTAL_VERTICES = 15;
  const int TOTAL_EDGES = 20;

  SECTION("generates input limited nodes") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_vertices() == TOTAL_VERTICES);
  }

  SECTION("generates input limited edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_edges() <= TOTAL_EDGES * 2);
  }

  SECTION("generates correct number of edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == TOTAL_EDGES * 2);
  }

  SECTION("early returns if exceeds max out degree") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, 1);
    REQUIRE(g.num_edges() <= TOTAL_VERTICES);
  }

  SECTION("early returns if there is not other candidate for a new edge when "
          "using no self edges mode") {
    generator.generate_random_graph(g, 1, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("chooses another target id with no self edges mode") {
    generator.generate_random_graph(g, 2, 100, -1, false);

    for (auto edgelist : g) {
      for (auto edge : edgelist) {
        REQUIRE(g.get_source(edge) != g.get_target(edge));
      }
    }
  }

  const int MAX_WEIGHT = 10;
  const int MIN_WEIGHT = 0;

  SECTION("generates weights within bounds") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MIN_WEIGHT, MAX_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION("swaps min and max weight if in bad order") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION(
      "swaps min and max weight if in bad order with fixed seed generator") {
    GraphGenerator fixed_seed_gen{0xdeadbeef};

    fixed_seed_gen.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights =
        fixed_seed_gen.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }
}

TEST_CASE("generate directed matrix graph",
          "[generator][matrix_graph][directed]") {
  AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED> g{};
  GraphGenerator generator;

  const int TOTAL_VERTICES = 15;
  const int TOTAL_EDGES = 20;

  SECTION("generates input limited nodes") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_vertices() == TOTAL_VERTICES);
  }

  SECTION("generates input limited edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_edges() == TOTAL_EDGES);
  }

  SECTION("early returns if exceeds max out degree") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, 1);
    REQUIRE(g.num_edges() == TOTAL_VERTICES);
  }

  SECTION("early returns if there is not other candidate for a new edge when "
          "using no self edges mode") {
    generator.generate_random_graph(g, 1, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("chooses another target id with no self edges mode") {
    generator.generate_random_graph(g, 2, 100, -1, false);

    for (auto edgelist : g) {
      for (auto edge : edgelist) {
        REQUIRE(g.get_source(edge) != g.get_target(edge));
      }
    }
  }

  const int MAX_WEIGHT = 10;
  const int MIN_WEIGHT = 0;

  SECTION("generates weights within bounds") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MIN_WEIGHT, MAX_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION("swaps min and max weight if in bad order") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION(
      "swaps min and max weight if in bad order with fixed seed generator") {
    GraphGenerator fixed_seed_gen{0xdeadbeef};

    fixed_seed_gen.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights =
        fixed_seed_gen.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }
}

TEST_CASE("generate undirected matrix graph",
          "[generator][matrix_graph][undirected]") {
  AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED> g{};
  GraphGenerator generator;

  const int TOTAL_VERTICES = 15;
  const int TOTAL_EDGES = 20;

  SECTION("generates input limited nodes") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_vertices() == TOTAL_VERTICES);
  }

  SECTION("generates input limited edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    REQUIRE(g.num_edges() <= TOTAL_EDGES * 2);
  }

  SECTION("generates correct number of edges") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == TOTAL_EDGES * 2);
  }

  SECTION("early returns if exceeds max out degree") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES, 1);
    REQUIRE(g.num_edges() <= TOTAL_VERTICES);
  }

  SECTION("early returns if there is not other candidate for a new edge when "
          "using no self edges mode") {
    generator.generate_random_graph(g, 1, TOTAL_EDGES, -1, false);
    REQUIRE(g.num_edges() == 0);
  }

  SECTION("chooses another target id with no self edges mode") {
    generator.generate_random_graph(g, 2, 100, -1, false);

    for (auto edgelist : g) {
      for (auto edge : edgelist) {
        REQUIRE(g.get_source(edge) != g.get_target(edge));
      }
    }
  }

  const int MAX_WEIGHT = 10;
  const int MIN_WEIGHT = 0;

  SECTION("generates weights within bounds") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MIN_WEIGHT, MAX_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION("swaps min and max weight if in bad order") {
    generator.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights = generator.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }

  SECTION(
      "swaps min and max weight if in bad order with fixed seed generator") {
    GraphGenerator fixed_seed_gen{0xdeadbeef};

    fixed_seed_gen.generate_random_graph(g, TOTAL_VERTICES, TOTAL_EDGES);
    auto weights =
        fixed_seed_gen.generate_random_weights(g, MAX_WEIGHT, MIN_WEIGHT);

    for (auto [edge, weight] : weights) {
      REQUIRE((weight >= MIN_WEIGHT && weight <= MAX_WEIGHT));
    }
  }
}

} // namespace graph_generator_test
