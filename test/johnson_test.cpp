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
#include "johnson.hpp"
#include "list_graph.hpp"

namespace johnson_test {
using namespace graph;
using namespace graphxx::algorithms;

TEST_CASE("Johnson shortest paths", "[johnson]") {
  AdjacencyListGraph<Directedness::DIRECTED> g{};

  std::unordered_map<DefaultIdType, int> weights;

  auto a = g.add_vertex(); // 0
  auto b = g.add_vertex(); // 1
  auto c = g.add_vertex(); // 2
  auto d = g.add_vertex(); // 3

  auto a_to_c = g.add_edge(a, c); // 0->2
  auto b_to_a = g.add_edge(b, a); // 1->0
  auto b_to_c = g.add_edge(b, c); // 1->2
  auto c_to_d = g.add_edge(c, d); // 2->3
  auto d_to_b = g.add_edge(d, b); // 3->1

  /*
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
  */

  SECTION("finds the shortest path length with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == a || tree[b][c].parent == b));
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  SECTION("find the shortest path length with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_c] = -2;

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == -2);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == -1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == -1);
    REQUIRE(tree[a][b].distance == 0);
  }

  SECTION("finds the parent nodes with one negative weight") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    weights[a_to_c] = -2;

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE(tree[b][c].parent == a);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }

  /*
    SECTION("throws on negative cycle found") {
      for (auto edge : g.edges()) {
        weights[edge] = 1;
      }

      weights[c_to_d] = -1;
      weights[d_to_b] = -1;

      REQUIRE_THROWS(floyd_warshall::visit(g, weights));
    }
  */

  auto c_to_a = g.add_edge(c, a); // 2->0
  auto d_to_d = g.add_edge(d, d); // 3->3

  /*
    <--------
    A------>C-------|
    ^       ^       |
    |       |       |
    B-------|       |
    ^               |
    |               |
    D<--------------|
   <->
  */

  SECTION("finds the shortest path length with all positive weights, now with "
          "cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].distance == 0);
    REQUIRE(tree[b][b].distance == 0);
    REQUIRE(tree[c][c].distance == 0);
    REQUIRE(tree[d][d].distance == 0);

    REQUIRE(tree[a][c].distance == 1);
    REQUIRE(tree[b][a].distance == 1);
    REQUIRE(tree[b][c].distance == 1);
    REQUIRE(tree[c][a].distance == 1);
    REQUIRE(tree[c][d].distance == 1);
    REQUIRE(tree[d][b].distance == 1);

    REQUIRE(tree[a][d].distance == 2);
    REQUIRE(tree[a][b].distance == 3);
  }

  SECTION("finds the parent nodes with all positive weights, now with cycles") {
    for (auto edge : g.edges()) {
      weights[edge] = 1;
    }

    auto tree = johnson::visit(g, weights);

    REQUIRE(tree[a][a].parent == INVALID_VERTEX);
    REQUIRE(tree[b][b].parent == INVALID_VERTEX);
    REQUIRE(tree[c][c].parent == INVALID_VERTEX);
    REQUIRE(tree[d][d].parent == INVALID_VERTEX);

    REQUIRE(tree[a][c].parent == a);
    REQUIRE(tree[b][a].parent == b);
    REQUIRE((tree[b][c].parent == b || tree[b][c].parent == a));
    REQUIRE(tree[c][a].parent == c);
    REQUIRE(tree[c][d].parent == c);
    REQUIRE(tree[d][b].parent == d);

    REQUIRE(tree[a][d].parent == c);
    REQUIRE(tree[a][b].parent == d);
  }
}
} // namespace johnson_test