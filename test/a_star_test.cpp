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

#include "a_star.hpp"
#include "base.hpp"
#include "catch.hpp"
#include "list_graph.hpp"

#include <map>
#include <tuple>

namespace a_star_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("A* shortest paths", "[a_star]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d, e, f, z };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;
  std::map<unsigned long, int> heuristic_weight;

  g.add_edge(a, b); // 0->1
  g.add_edge(a, c); // 0->2
  g.add_edge(b, f); // 1->5
  g.add_edge(b, e); // 1->4
  g.add_edge(c, d); // 2->3
  g.add_edge(c, e); // 2->4
  g.add_edge(d, e); // 3->4
  g.add_edge(e, z); // 4->6
  g.add_edge(f, z); // 5->6

  /*
    A-->B-->F---------->Z
    |   |               ^
    |   ---------->E----^
    |              ^
    |   -----------^
    |   |          ^
    --->C-->D------^
  */

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.source(e), g.target(e)}];
  };

  auto get_heuristic = [&](typename Graph::Id v) {
    return heuristic_weight[v];
  };

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, b}] = -1;

    REQUIRE_THROWS(a_star::visit(g, a, z, get_heuristic, get_weight));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, c}] = 2;
    weight[{b, f}] = 2;

    auto distances = a_star::visit(g, a, z, get_heuristic, get_weight);

    REQUIRE(distances.size() == 4);
    REQUIRE(distances[0].distance == 0); // a
    REQUIRE(distances[1].distance == 1); // b
    REQUIRE(distances[2].distance == 2); // e
    REQUIRE(distances[3].distance == 3); // z
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto distances = a_star::visit(g, a, z, get_heuristic, get_weight);

    REQUIRE(distances.size() == 4);
    REQUIRE(distances[0].parent == INVALID_VERTEX<Graph>); // a
    REQUIRE(distances[1].parent == a);                     // b
    REQUIRE(distances[2].parent == b);                     // e
    REQUIRE(distances[3].parent == e);                     // z
  }

  g.add_edge(a, a); // 0->0
  g.add_edge(d, d); // 3->3
  g.add_edge(e, c); // 4->2

  /*
   <->
    A-->B-->F---------->Z
    |   |               ^
    |   ---------->E|----^
    |              ^|
    |   -----------^|
    |   |  <->     ^|
    --->C-->D------^|
        ^-----------|
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, c}] = 2;
    weight[{b, f}] = 2;
    weight[{d, d}] = 1;
    weight[{e, c}] = 1;

    auto distances = a_star::visit(g, a, z, get_heuristic, get_weight);

    REQUIRE(distances.size() == 4);
    REQUIRE(distances[0].distance == 0); // a
    REQUIRE(distances[1].distance == 1); // b
    REQUIRE(distances[2].distance == 2); // e
    REQUIRE(distances[3].distance == 3); // z
  }

  SECTION("finds the previous hop with all positive weights, now with cycles") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto distances = a_star::visit(g, a, z, get_heuristic, get_weight);

    REQUIRE(distances.size() == 4);
    REQUIRE(distances[0].parent == INVALID_VERTEX<Graph>); // a
    REQUIRE(distances[1].parent == a);                     // b
    REQUIRE(distances[2].parent == b);                     // e
    REQUIRE(distances[3].parent == e);                     // z
  }

  SECTION("algorithm failure") {
    g.add_vertex(g.num_vertices());

    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      heuristic_weight[vertex] = 1;
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto distances =
        a_star::visit(g, a, g.num_vertices() - 1, get_heuristic, get_weight);

    REQUIRE(distances.size() == 0);
  }
}
} // namespace a_star_test