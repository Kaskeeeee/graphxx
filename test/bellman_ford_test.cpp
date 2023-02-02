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
#include "bellman_ford.hpp"
#include "catch.hpp"
#include "list_graph.hpp"

#include <map>
#include <tuple>

namespace bellman_ford_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Bellman ford shortest paths", "[bellman-ford]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d, e };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;

  g.add_edge(a, b); // 0->1
  g.add_edge(a, d); // 0->3
  g.add_edge(b, c); // 1->2
  g.add_edge(d, e); // 3->4
  g.add_edge(e, c); // 4->2
  g.add_edge(c, e); // 2->4

  /*
    A--->B--->C
    |    |    ^|
    |    v    |v
    ---->D--->E
  */

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.get_source(e), g.get_target(e)}];
  };

  SECTION("throws on negative cycle found") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.get_source(edge), g.get_target(edge)}] = 1;
      }
    }

    weight[{e, c}] = -10;

    REQUIRE_THROWS(bellman_ford::visit(g, a, get_weight));
  }

  SECTION("find the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.get_source(edge), g.get_target(edge)}] = 1;
      }
    }

    auto distances = bellman_ford::visit(g, a, get_weight);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 2);
    REQUIRE(distances[d].distance == 1);
    REQUIRE(distances[e].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.get_source(edge), g.get_target(edge)}] = 1;
      }
    }

    auto distances = bellman_ford::visit(g, a, get_weight);

    REQUIRE(distances[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(distances[b].parent == a);
    REQUIRE(distances[c].parent == b);
    REQUIRE(distances[d].parent == a);
    REQUIRE(distances[e].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.get_source(edge), g.get_target(edge)}] = 1;
      }
    }

    weight[{d, e}] = -1;

    auto distances = bellman_ford::visit(g, a, get_weight);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 1);
    REQUIRE(distances[e].distance == 0);
  }

  SECTION("find the previous hop with one negative weight") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.get_source(edge), g.get_target(edge)}] = 1;
      }
    }

    weight[{d, e}] = -1;

    auto distances = bellman_ford::visit(g, a, get_weight);

    REQUIRE(distances[a].parent == INVALID_VERTEX<Graph>);
    REQUIRE(distances[b].parent == a);
    REQUIRE(distances[c].parent == e);
    REQUIRE(distances[d].parent == a);
    REQUIRE(distances[e].parent == d);
  }
}
} // namespace bellman_ford_test