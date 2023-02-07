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
#include "ford_fulkerson.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"

#include <vector>

namespace ford_fulkerson_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Ford Fulkerson maximum flow for directed list graph",
          "[ford_fulkerson][list_graph][directed]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a);
  graph.add_edge(s, d);
  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(b, t);
  graph.add_edge(c, b);
  graph.add_edge(c, t);
  graph.add_edge(d, a);
  graph.add_edge(d, c);

  std::vector<std::vector<int32_t>> capacities(
      graph.num_vertices(), std::vector<int32_t>(graph.num_vertices()));

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
  */

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        capacities[graph.get_source(edge)][graph.get_target(edge)] = 1;
      }
    }

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 2);
  }

  SECTION("finds the maximum possible flow") {
    capacities[s][a] = 7;
    capacities[s][d] = 4;
    capacities[a][b] = 5;
    capacities[a][c] = 3;
    capacities[b][t] = 8;
    capacities[c][b] = 3;
    capacities[c][t] = 5;
    capacities[d][a] = 3;
    capacities[d][c] = 2;

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 10);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for directed matrix graph",
          "[ford_fulkerson][matrix_graph][directed]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::DIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a);
  graph.add_edge(s, d);
  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(b, t);
  graph.add_edge(c, b);
  graph.add_edge(c, t);
  graph.add_edge(d, a);
  graph.add_edge(d, c);

  std::vector<std::vector<int32_t>> capacities(
      graph.num_vertices(), std::vector<int32_t>(graph.num_vertices()));

  /*
    S----->A----->B-----v
     ----->D------^     T
            ------>C----^
  */

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        capacities[graph.get_source(edge)][graph.get_target(edge)] = 1;
      }
    }

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 2);
  }

  SECTION("finds the maximum possible flow") {
    capacities[s][a] = 7;
    capacities[s][d] = 4;
    capacities[a][b] = 5;
    capacities[a][c] = 3;
    capacities[b][t] = 8;
    capacities[c][b] = 3;
    capacities[c][t] = 5;
    capacities[d][a] = 3;
    capacities[d][c] = 2;

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 10);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for undirected list graph",
          "[ford_fulkerson][list_graph][undirected]") {
  using Graph =
      AdjacencyListGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a);
  graph.add_edge(s, d);
  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(b, t);
  graph.add_edge(c, b);
  graph.add_edge(c, t);
  graph.add_edge(d, a);
  graph.add_edge(d, c);

  std::vector<std::vector<int32_t>> capacities(
      graph.num_vertices(), std::vector<int32_t>(graph.num_vertices()));

  /*
    S------A------B------|
     ------D------|      T
            --------C----|
  */

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        capacities[graph.get_source(edge)][graph.get_target(edge)] = 1;
      }
    }

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 2);
  }

  SECTION("finds the maximum possible flow") {
    capacities[s][a] = 7;
    capacities[s][d] = 4;
    capacities[a][b] = 5;
    capacities[a][c] = 3;
    capacities[b][t] = 8;
    capacities[c][b] = 3;
    capacities[c][t] = 5;
    capacities[d][a] = 3;
    capacities[d][c] = 2;

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 10);
  }
}

TEST_CASE("Ford Fulkerson maximum flow for undirected matrix graph",
          "[ford_fulkerson][matrix_graph][undirected]") {
  using Graph =
      AdjacencyMatrixGraph<unsigned long, Directedness::UNDIRECTED, int>;
  Graph graph{};

  enum vertices { s, a, b, c, d, t };

  graph.add_edge(s, a);
  graph.add_edge(s, d);
  graph.add_edge(a, b);
  graph.add_edge(a, c);
  graph.add_edge(b, t);
  graph.add_edge(c, b);
  graph.add_edge(c, t);
  graph.add_edge(d, a);
  graph.add_edge(d, c);

  std::vector<std::vector<int32_t>> capacities(
      graph.num_vertices(), std::vector<int32_t>(graph.num_vertices()));

  /*
    S------A------B------|
     ------D------|      T
            --------C----|
  */

  SECTION("finds the maximum possible flow with simple weights") {
    for (size_t vertex = 0; vertex < graph.num_vertices(); vertex++) {
      auto out_edge_list = graph[vertex];
      for (auto edge : out_edge_list) {
        capacities[graph.get_source(edge)][graph.get_target(edge)] = 1;
      }
    }

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 2);
  }

  SECTION("finds the maximum possible flow") {
    capacities[s][a] = 7;
    capacities[s][d] = 4;
    capacities[a][b] = 5;
    capacities[a][c] = 3;
    capacities[b][t] = 8;
    capacities[c][b] = 3;
    capacities[c][t] = 5;
    capacities[d][a] = 3;
    capacities[d][c] = 2;

    auto result = ford_fulkerson(graph, s, t, capacities);

    REQUIRE(result == 10);
  }
}
} // namespace ford_fulkerson_test