/**
 * @file This file contains the unit tests for Tarjan algorithm
 *
 * @copyright Copyright © 2023 Matteo Cavaliere, Cristiano Di Bari, Michele Quaresmini, Andrea Cinelli. All rights reserved.
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
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include "tarjan.hpp"

namespace tarjan_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Tarjan for strongly connected components for directed list graph",
          "[tarjan][list_graph][directed]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(d, b);
  graph.add_edge(d, c);
  graph.add_edge(d, e);
  graph.add_edge(e, d);
  graph.add_edge(e, f);

  /*
    A       B       C       D       E       F
    |------->
            |------->
    <---------------|
            <-------<-------|------->
                            <-------|------->
  */

  SECTION("detects strongly connected components") {
    auto scc = tarjan(graph);

    REQUIRE(scc.size() == 3);
  }

  SECTION("check if strongly connected component are the correct") {
    auto scc = tarjan(graph);

    REQUIRE(scc[0][0] == c);
    REQUIRE(scc[0][1] == b);
    REQUIRE(scc[0][2] == a);

    REQUIRE(scc[1][0] == f);

    REQUIRE(scc[2][0] == e);
    REQUIRE(scc[2][1] == d);
  }
}

TEST_CASE("Tarjan for strongly connected components for directed matrix graph",
          "[tarjan][matrix_graph][directed]") {
  using Graph = AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(d, b);
  graph.add_edge(d, c);
  graph.add_edge(d, e);
  graph.add_edge(e, d);
  graph.add_edge(e, f);

  /*
    A       B       C       D       E       F
    |------->
            |------->
    <---------------|
            <-------<-------|------->
                            <-------|------->
  */

  SECTION("detects strongly connected components") {
    auto scc = tarjan(graph);

    REQUIRE(scc.size() == 3);
  }

  SECTION("check if strongly connected component are the correct") {
    auto scc = tarjan(graph);

    REQUIRE(scc[0][0] == c);
    REQUIRE(scc[0][1] == b);
    REQUIRE(scc[0][2] == a);

    REQUIRE(scc[1][0] == f);

    REQUIRE(scc[2][0] == e);
    REQUIRE(scc[2][1] == d);
  }
}

TEST_CASE("Tarjan for strongly connected components for undirected list graph",
          "[tarjan][list_graph][undirected]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(d, b);
  graph.add_edge(d, c);
  graph.add_edge(d, e);
  graph.add_edge(e, f);

  /*
    A       B       C       D       E       F
    |-------|
            |-------|
    |---------------|
            |-------|-------|-------|
                                    |-------|
  */

  SECTION("detects strongly connected components") {
    auto scc = tarjan(graph);

    REQUIRE(scc.size() == 1);
  }
}

TEST_CASE(
    "Tarjan for strongly connected components for undirected matrix graph",
    "[tarjan][matrix_graph][undirected]") {
  using Graph = AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED>;
  Graph graph{};

  enum vertices { a, b, c, d, e, f };

  graph.add_edge(a, b);
  graph.add_edge(b, c);
  graph.add_edge(c, a);
  graph.add_edge(d, b);
  graph.add_edge(d, c);
  graph.add_edge(d, e);
  graph.add_edge(e, f);

  /*
    A       B       C       D       E       F
    |-------|
            |-------|
    |---------------|
            |-------|-------|-------|
                                    |-------|
  */

  SECTION("detects strongly connected components") {
    auto scc = tarjan(graph);

    REQUIRE(scc.size() == 1);
  }
}
} // namespace tarjan_test