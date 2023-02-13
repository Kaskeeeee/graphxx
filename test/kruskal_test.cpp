/**
 * @file This file contains the unit tests for Kruskal algorithm
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
#include "kruskal.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

namespace kruskal_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Kruskal minimum spanning tree for directed list graph",
          "[kruskal][list_graph][directed]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(a, d);
  graph.add_edge(b, a);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(c, d);
  graph.add_edge(d, a);

  /*
     A       B       C       D
     |-------^
     |---------------^
     |-----------------------^
     ^-------|-------^
     ^---------------|-------^
     ^-----------------------|
 */

  graph.set_attributes(a, b, {2});
  graph.set_attributes(a, c, {1});
  graph.set_attributes(a, d, {4});
  graph.set_attributes(b, a, {2});
  graph.set_attributes(c, a, {1});
  graph.set_attributes(d, a, {4});
  graph.set_attributes(b, c, {3});
  graph.set_attributes(c, d, {9});

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal(graph);

    REQUIRE(vector.size() == 3);
  }

  SECTION("check if edges founded are correct") {
    auto vector = kruskal(graph);

    REQUIRE(((graph.get_source(vector[0]) == a &&
              graph.get_target(vector[0]) == c) ||
             (graph.get_source(vector[0]) == c &&
              graph.get_target(vector[0]) == a)));
    REQUIRE(((graph.get_source(vector[1]) == a &&
              graph.get_target(vector[1]) == b) ||
             (graph.get_source(vector[1]) == b &&
              graph.get_target(vector[1]) == a)));
    REQUIRE(((graph.get_source(vector[2]) == a &&
              graph.get_target(vector[2]) == d) ||
             (graph.get_source(vector[2]) == d &&
              graph.get_target(vector[2]) == a)));
  }

  SECTION("check if no spanning tree in worst case") {
    Graph graph2{};

    graph2.add_edge(a, a);
    graph.set_attributes(a, a, {1});

    auto vector = kruskal(graph2);

    REQUIRE(vector.size() == 0);
  }
}

TEST_CASE("Kruskal minimum spanning tree for directed matrix graph",
          "[kruskal][matrix_graph][directed]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(a, d);
  graph.add_edge(b, a);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(c, d);
  graph.add_edge(d, a);

  /*
     A       B       C       D
     |-------^
     |---------------^
     |-----------------------^
     ^-------|-------^
     ^---------------|-------^
     ^-----------------------|
 */

  graph.set_attributes(a, b, {2});
  graph.set_attributes(a, c, {1});
  graph.set_attributes(a, d, {4});
  graph.set_attributes(b, a, {2});
  graph.set_attributes(c, a, {1});
  graph.set_attributes(d, a, {4});
  graph.set_attributes(b, c, {3});
  graph.set_attributes(c, d, {9});

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal(graph);

    REQUIRE(vector.size() == 3);
  }

  SECTION("check if edges founded are correct") {
    auto vector = kruskal(graph);

    REQUIRE(((graph.get_source(vector[0]) == a &&
              graph.get_target(vector[0]) == c) ||
             (graph.get_source(vector[0]) == c &&
              graph.get_target(vector[0]) == a)));
    REQUIRE(((graph.get_source(vector[1]) == a &&
              graph.get_target(vector[1]) == b) ||
             (graph.get_source(vector[1]) == b &&
              graph.get_target(vector[1]) == a)));
    REQUIRE(((graph.get_source(vector[2]) == a &&
              graph.get_target(vector[2]) == d) ||
             (graph.get_source(vector[2]) == d &&
              graph.get_target(vector[2]) == a)));
  }

  SECTION("check if no spanning tree in worst case") {
    Graph graph2{};

    graph2.add_edge(a, a);
    graph.set_attributes(a, a, {1});

    auto vector = kruskal(graph2);

    REQUIRE(vector.size() == 0);
  }
}

TEST_CASE("Kruskal minimum spanning tree for undirected list graphs",
          "[kruskal][list_graph][undirected]") {
  using Graph =
      AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(a, d);
  graph.add_edge(b, c);
  graph.add_edge(c, d);

  /*
      A       B       C       D
      |-------|
              |-------|
      |---------------|-------|
      |-----------------------|
  */

  graph.set_attributes(a, b, {2});
  graph.set_attributes(a, c, {1});
  graph.set_attributes(a, d, {4});
  graph.set_attributes(b, c, {3});
  graph.set_attributes(c, d, {9});

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal(graph);

    REQUIRE(vector.size() == 3);
  }

  SECTION("check if edges founded are correct") {
    auto vector = kruskal(graph);

    REQUIRE(((graph.get_source(vector[0]) == a &&
              graph.get_target(vector[0]) == c) ||
             (graph.get_source(vector[0]) == c &&
              graph.get_target(vector[0]) == a)));
    REQUIRE(((graph.get_source(vector[1]) == a &&
              graph.get_target(vector[1]) == b) ||
             (graph.get_source(vector[1]) == b &&
              graph.get_target(vector[1]) == a)));
    REQUIRE(((graph.get_source(vector[2]) == a &&
              graph.get_target(vector[2]) == d) ||
             (graph.get_source(vector[2]) == d &&
              graph.get_target(vector[2]) == a)));
  }

  SECTION("check if no spanning tree in worst case") {
    Graph graph2{};

    graph2.add_edge(a, a);
    graph.set_attributes(a, a, {1});

    auto vector = kruskal(graph2);

    REQUIRE(vector.size() == 0);
  }
}

TEST_CASE("Kruskal minimum spanning tree for undirected matrix graph",
          "[kruskal][matrix_graph][undirected]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { a, b, c, d };

  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(a, d);
  graph.add_edge(b, c);
  graph.add_edge(c, d);

  /*
      A       B       C       D
      |-------|
              |-------|
      |---------------|-------|
      |-----------------------|
  */

  graph.set_attributes(a, b, {2});
  graph.set_attributes(a, c, {1});
  graph.set_attributes(a, d, {4});
  graph.set_attributes(b, c, {3});
  graph.set_attributes(c, d, {9});

  SECTION("find mimimum spanning tree") {
    auto vector = kruskal(graph);

    REQUIRE(vector.size() == 3);
  }

  SECTION("check if edges founded are correct") {
    auto vector = kruskal(graph);

    REQUIRE(((graph.get_source(vector[0]) == a &&
              graph.get_target(vector[0]) == c) ||
             (graph.get_source(vector[0]) == c &&
              graph.get_target(vector[0]) == a)));
    REQUIRE(((graph.get_source(vector[1]) == a &&
              graph.get_target(vector[1]) == b) ||
             (graph.get_source(vector[1]) == b &&
              graph.get_target(vector[1]) == a)));
    REQUIRE(((graph.get_source(vector[2]) == a &&
              graph.get_target(vector[2]) == d) ||
             (graph.get_source(vector[2]) == d &&
              graph.get_target(vector[2]) == a)));
  }

  SECTION("check if no spanning tree in worst case") {
    Graph graph2{};

    graph2.add_edge(a, a);
    graph.set_attributes(a, a, {1});

    auto vector = kruskal(graph2);

    REQUIRE(vector.size() == 0);
  }
}
} // namespace kruskal_test