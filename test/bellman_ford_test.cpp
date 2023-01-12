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
#include "dijkstra.hpp"
#include "list_graph.hpp"

#include <iostream>
#include <unordered_map>

namespace bellman_ford_test {
using namespace graph;
using namespace graph::algorithms;

TEST_CASE("Bellman ford shortest paths", "[bellman-ford]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<Id, int> weights;

  auto a = g.add_vertex(); // 0
  auto b = g.add_vertex(); // 1
  auto c = g.add_vertex(); // 2
  auto d = g.add_vertex(); // 3
  auto e = g.add_vertex(); // 4

  auto a_to_b = g.add_edge(a, b); // 0->1
  auto a_to_d = g.add_edge(a, d); // 0->3
  auto b_to_c = g.add_edge(b, c); // 1->2
  auto d_to_e = g.add_edge(d, e); // 3->4
  auto e_to_c = g.add_edge(e, c); // 4->2
  auto c_to_e = g.add_edge(c, e); // 2->4

  /*
    A--->B--->C
    |    |    ^|
    |    v    |v
    ---->D--->E
  */

  SECTION("throws on negative cycle found") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[e_to_c] = -10;

    REQUIRE_THROWS(bellman_ford::visit(g, a, weights));
  }

  SECTION("find the shortest path length with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = bellman_ford::visit(g, a, weights);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 2);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 2);
  }

  SECTION("finds the previous hop with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = bellman_ford::visit(g, a, weights);

    REQUIRE(tree[a].parent == INVALID_VERTEX);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == b);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[d_to_e] = -1;

    auto tree = bellman_ford::visit(g, a, weights);

    REQUIRE(tree[a].distance == 0);
    REQUIRE(tree[b].distance == 1);
    REQUIRE(tree[c].distance == 1);
    REQUIRE(tree[d].distance == 1);
    REQUIRE(tree[e].distance == 0);
  }

  SECTION("find the previous hop with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[d_to_e] = -1;

    auto tree = bellman_ford::visit(g, a, weights);

    REQUIRE(tree[a].parent == INVALID_VERTEX);
    REQUIRE(tree[b].parent == a);
    REQUIRE(tree[c].parent == e);
    REQUIRE(tree[d].parent == a);
    REQUIRE(tree[e].parent == d);
  }
}
} // namespace bellman_ford_test