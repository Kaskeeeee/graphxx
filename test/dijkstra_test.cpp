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
#include "dijkstra.hpp"
#include "list_graph.hpp"

#include <map>
#include <tuple>

namespace dijsktra_test {
using namespace graphxx;
using namespace graphxx::algorithms;

TEST_CASE("Dijkstra shortest paths", "[dijsktra]") {
  using Graph = AdjacencyListGraph<unsigned long, Directedness::DIRECTED>;
  Graph g{};

  enum vertices { a, b, c, d, e, f };

  std::map<std::tuple<unsigned long, unsigned long>, int> weight;

  g.add_edge(a, b); // 0->1
  g.add_edge(a, c); // 0->2
  g.add_edge(b, c); // 1->2
  g.add_edge(c, d); // 2->3
  g.add_edge(c, e); // 2->4
  g.add_edge(c, f); // 2->5
  g.add_edge(d, f); // 3->5
  g.add_edge(e, f); // 4->5

  /*
    A-->B
    |   |
    |   v
    --->C-->D------v
        --->E------v
        ---------->F
  */

  auto get_weight = [&](typename Graph::Edge e) {
    return weight[{g.source(e), g.target(e)}];
  };

  SECTION("throws on negative edge found") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{a, b}] = -1;

    REQUIRE_THROWS(dijkstra::visit(g, a, get_weight));
  }

  SECTION("finds the shortest path length with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{c, f}] = 7;

    auto distances = dijkstra::visit(g, a, get_weight);

    REQUIRE(distances[a].distance == 0);
    REQUIRE(distances[b].distance == 1);
    REQUIRE(distances[c].distance == 1);
    REQUIRE(distances[d].distance == 2);
    REQUIRE(distances[e].distance == 2);
    REQUIRE(distances[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto result = dijkstra::visit(g, a, get_weight);

    REQUIRE(result[a].parent == a);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }

  g.add_edge(a, a); // 0->0
  g.add_edge(d, d); // 3->3
  g.add_edge(e, c); // 4->2

  /*
   <->
    A-->B
    |   |
    |   v  <->
    --->C-->D-------v
        --->E------>F
        <----
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    weight[{c, f}] = 7;
    weight[{d, d}] = 0;
    weight[{e, c}] = 0;

    auto result = dijkstra::visit(g, a, get_weight);

    REQUIRE(result[a].distance == 0);
    REQUIRE(result[b].distance == 1);
    REQUIRE(result[c].distance == 1);
    REQUIRE(result[d].distance == 2);
    REQUIRE(result[e].distance == 2);
    REQUIRE(result[f].distance == 3);
  }

  SECTION("finds the previous hop with all positive weights, now with cycles") {
    for (size_t vertex = 0; vertex < g.num_vertices(); vertex++) {
      auto out_edge_list = g[vertex];
      for (auto edge : out_edge_list) {
        weight[{g.source(edge), g.target(edge)}] = 1;
      }
    }

    auto result = dijkstra::visit(g, a, get_weight);

    REQUIRE(result[a].parent == a);
    REQUIRE(result[b].parent == a);
    REQUIRE(result[c].parent == a);
    REQUIRE(result[d].parent == c);
    REQUIRE(result[e].parent == c);
    REQUIRE(result[f].parent == c);
  }
}
} // namespace dijsktra_test